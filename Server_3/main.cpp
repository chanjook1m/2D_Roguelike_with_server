#include <boost/asio.hpp>

#include <thread>
#include <atomic>
#include <memory>
#include <iostream>

#include <boost/serialization/serialization.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/base_object.hpp>

#include "net.hpp"



enum types
{
	COIN = 1,
	POWERUP,
};

std::vector<std::shared_ptr<boost::asio::ip::tcp::socket>> clients;
std::vector<net::Player> players;
std::vector<net::Projectile> projectiles;
std::vector<net::Enemy> enemies;
std::vector<net::Item> items;
std::vector<net::Wall> walls;

net::Projectile projectile;
net::Enemy enemy;
net::Item item;
net::Wall wall;

unsigned int id = 1;
unsigned int projectile_id = 0;
int wall_id = 0;
int enemy_id = 0;
int item_id = 0;
int counter, counter2;


const short multicast_port = 30001;

class sender
{
public:
	sender(boost::asio::io_context& io_context,
		const boost::asio::ip::address& multicast_address)
		: endpoint_(multicast_address, multicast_port),
		socket_(io_context, endpoint_.protocol()),
		timer_(io_context),
		message_count_(0)
	{
		socket_.async_send_to(
			boost::asio::buffer(message_), endpoint_,
			boost::bind(&sender::handle_send_to, this,
				boost::asio::placeholders::error));
	}

	void handle_send_to(const boost::system::error_code& error)
	{
		if (!error)
		{
			timer_.expires_from_now(boost::posix_time::milliseconds(10));
			timer_.async_wait(
				boost::bind(&sender::handle_timeout, this,
					boost::asio::placeholders::error));
		}
	}

	void handle_timeout(const boost::system::error_code& error)
	{
		if (!error)
		{
			// update objects
			// items
			for (int i = 0; i < items.size(); i++)
			{
				// players - item collision
				for (int j = 0; j < players.size(); j++)
				{
					if (net::detectCollision(players[j], items[i]))
					{
						if (items[i].inShop == false)
						{
							if (items[i].type == COIN)
							{
								players[j].score += 20;
							}
							else if (items[i].type == POWERUP)
							{

								if (players[j].powerUpLevel < players[j].maxPowerUpLevel)
								{

									players[j].powerUpLevel++;
								}
							}
							items[i].isAlive = false;
						}
						else // shopitem
						{
							if (players[j].score >= items[i].cost && players[j].powerUpLevel < 5)
							{
								int num = players[j].score / items[i].cost;

								if (players[j].powerUpLevel + num >= 5)
								{
									num = 5 - players[j].powerUpLevel;
									players[j].powerUpLevel = 5;
								}
								else
								{
									players[j].powerUpLevel += num;
								}
								players[j].score -= items[i].cost * num;

							}
						}
					}
				}

				if (items[i].isAlive == false)
				{
					items.erase(items.begin() + i);
				}
			}

			// walls
			for (int i = 0; i < walls.size(); i++)
			{

				// enemy - wall collision
				for (int j = 0; j < enemies.size(); j++)
				{
					if (net::detectCollision(enemies[j], walls[i]))
					{
						if (enemies[j].direction == 1)
						{
							enemies[j].canMoveUp = false;
							enemies[j].direction = net::generateRandom(4);
							enemies[j].temp_y += enemies[j].velocity;
						}
						else if (enemies[j].direction == 2)
						{
							enemies[j].canMoveDown = false;

							enemies[j].direction = net::generateRandom(4);
							enemies[j].temp_y -= enemies[j].velocity;
						}
						else if (enemies[j].direction == 3)
						{
							enemies[j].canMoveLeft = false;
							enemies[j].direction = net::generateRandom(4);
							enemies[j].temp_x += enemies[j].velocity;
						}
						else if (enemies[j].direction == 4)
						{
							enemies[j].canMoveRight = false;
							enemies[j].direction = net::generateRandom(4);
							enemies[j].temp_x -= enemies[j].velocity;
						}
					}
				}

				// projectile - wall collision
				for (int j = 0; j < projectiles.size(); j++)
				{
					if (net::detectCollision(walls[i], projectiles[j]))
					{


						projectiles[j].isAlive = false;
						if (walls[i].destructible == true)
						{
							walls[i].hp -= projectiles[j].attackDamage;
						}

						if (walls[i].hp <= 0)
						{
							walls[i].isAlive = false;
						}
					}
				}

				if (walls[i].isAlive == false)
				{
					walls.erase(walls.begin() + i);
				}
			}

			// projectile
			for (int i = 0; i < projectiles.size(); i++)
			{
				projectiles[i].update();

				// projectile - enemy collision
				for (int j = 0; j < enemies.size(); j++)
				{
					if (net::detectCollision(enemies[j], projectiles[i]))
					{
						projectiles[i].isAlive = false;
						enemies[j].aggroedBy = projectiles[i].id;
						enemies[j].hp -= projectiles[i].attackDamage;

						if (enemies[j].hp <= 0)
						{
							enemies[j].isAlive = false;
							enemies[j].aggroedBy = 0;
						}
					}
				}

				//// projectile - players collision
				//for (int j = 0; j < players.size(); j++)
				//{
				//	if (net::detectCollision(projectiles[i], players[j]))
				//	{
				//		projectiles[i].isAlive = false;
				//		players[j].hp -= projectiles[i].attackDamage;

				//		if (players[j].hp <= 0)
				//		{
				//			players[j].isAlive = false;
				//		}
				//	}
				//}



				if (projectiles[i].isAlive == false)
				{
					projectiles.erase(projectiles.begin() + i);
				}
			}

			// enemyMonster
			for (int i = 0; i < enemies.size(); i++)
			{
				enemies[i].update();

				// enemyMonster - player collision
				for (int j = 0; j < players.size(); j++)
				{
					if (abs(players[j].collisionRect_y - enemies[i].collisionRect_y) <= 100 &&
						abs(players[j].collisionRect_x - enemies[i].collisionRect_x) <= 100)
					{
						enemies[i].aggroedBy = players[j].id;
					}
					else
					{
						enemies[i].aggroedBy = 0;
					}

					if (net::detectCollision(players[j], enemies[i]))
					{
						players[j].hp -= enemies[i].attackDamage;

						if (players[j].hp < 0)
							players[j].hp = 0;

						if (players[j].powerUpLevel > 1)
						{
							players[j].powerUpLevel--;
						}


					}
				}

				// AI
				for (int j = 0; j < players.size(); j++)
				{

					if (enemies[i].aggroedBy == players[j].id)
					{
						if (players[j].collisionRect_x < enemies[i].collisionRect_x &&
							players[j].collisionRect_y - enemies[i].collisionRect_y <= 40)
						{
							enemies[i].direction = 3;
						}
						if (players[j].collisionRect_x > enemies[i].collisionRect_x &&
							players[j].collisionRect_y - enemies[i].collisionRect_y <= 40)
						{
							enemies[i].direction = 4;
						}
						if (players[j].collisionRect_y < enemies[i].collisionRect_y &&
							players[j].collisionRect_x - enemies[i].collisionRect_x <= 40)
						{
							enemies[i].direction = 1;
						}
						if (players[j].collisionRect_y > enemies[i].collisionRect_y &&
							players[j].collisionRect_x - enemies[i].collisionRect_x <= 80)
						{
							enemies[i].direction = 2;
						}
					}
				}




				if (enemies[i].isAlive == false)
				{
					// generate item           
					item.isAlive = true;
					int random = net::generateRandom(2);

					if (random == 1)
					{
						item.collisionRect_x = enemies[i].collisionRect_x;
						item.collisionRect_y = enemies[i].collisionRect_y;
						item.type = COIN;
						item.scaleValue = item.type == COIN ? 0.2 : 0.4;
						item.id = item_id++;
						items.push_back(item);
					}
					else if (random == 2)
					{
						item.collisionRect_x = enemies[i].collisionRect_x;
						item.collisionRect_y = enemies[i].collisionRect_y;
						item.type = POWERUP;
						item.scaleValue = item.type == COIN ? 0.2 : 0.4;
						item.id = item_id++;
						items.push_back(item);
					}
					enemies.erase(enemies.begin() + i);
				}
			}


			boost::asio::streambuf buf;
			net::ServerPacket pack;

			if (players.size() > 0)
				pack.players = players;
			if (projectiles.size() > 0)
				pack.projectiles = projectiles;
			if (enemies.size() > 0)
				pack.enemies = enemies;
			if (items.size() > 0)
				pack.items = items;
			if (walls.size() > 0)
				pack.walls = walls;

			std::cout << "[INFO] player size : " << players.size() << std::endl;
			std::cout << "[INFO] projectile size : " << projectiles.size() << std::endl;
			std::cout << "[INFO] enemy size : " << enemies.size() << std::endl;
			std::cout << "[INFO] wall size : " << walls.size() << std::endl;
			std::cout << "[INFO] item size : " << items.size() << std::endl;


			std::ostream oss(&buf);

			pack.save(oss);
			boost::asio::streambuf::const_buffers_type bufs = buf.data();
			std::string str(boost::asio::buffers_begin(bufs),
				boost::asio::buffers_begin(bufs) + bufs.size());
			message_ = str;//"response...";
			//m_response += "\r\n";
			socket_.async_send_to(
				boost::asio::buffer(message_), endpoint_,
				boost::bind(&sender::handle_send_to, this,
					boost::asio::placeholders::error));
		}
	}

private:
	boost::asio::ip::udp::endpoint endpoint_;
	boost::asio::ip::udp::socket socket_;
	boost::asio::deadline_timer timer_;
	int message_count_;
	std::string message_;
};


class Service
{
public:
	Service(std::shared_ptr<boost::asio::ip::tcp::socket> sock) :
		m_sock(sock)
	{

	}

	void StartHandling()
	{
		boost::asio::async_read(*m_sock.get(),
			m_request,
			boost::asio::transfer_at_least(1),
			[this](const boost::system::error_code& ec,
				std::size_t bytes_transferred)
			{
				onRequestReceived(ec, bytes_transferred);
			}
		);

	}

	void StartHandling2()
	{
		// update objects
		// items
		for (int i = 0; i < items.size(); i++)
		{
			// players - item collision
			for (int j = 0; j < players.size(); j++)
			{
				if (net::detectCollision(players[j], items[i]))
				{
					if (items[i].inShop == false)
					{
						if (items[i].type == COIN)
						{
							players[j].score += 20;
						}
						else if (items[i].type == POWERUP)
						{

							if (players[j].powerUpLevel < players[j].maxPowerUpLevel)
							{

								players[j].powerUpLevel++;
							}
						}
						items[i].isAlive = false;
					}
					else // shopitem
					{
						if (players[j].score >= items[i].cost && players[j].powerUpLevel < 5)
						{
							int num = players[j].score / items[i].cost;

							if (players[j].powerUpLevel + num >= 5)
							{
								num = 5 - players[j].powerUpLevel;
								players[j].powerUpLevel = 5;
							}
							else
							{
								players[j].powerUpLevel += num;
							}
							players[j].score -= items[i].cost * num;

						}
					}
				}
			}

			if (items[i].isAlive == false)
			{
				items.erase(items.begin() + i);
			}
		}

		// walls
		for (int i = 0; i < walls.size(); i++)
		{

			// enemy - wall collision
			for (int j = 0; j < enemies.size(); j++)
			{
				if (net::detectCollision(enemies[j], walls[i]))
				{
					if (enemies[j].direction == 1)
					{
						enemies[j].canMoveUp = false;
						enemies[j].direction = net::generateRandom(4);
						enemies[j].temp_y += enemies[j].velocity;
					}
					else if (enemies[j].direction == 2)
					{
						enemies[j].canMoveDown = false;

						enemies[j].direction = net::generateRandom(4);
						enemies[j].temp_y -= enemies[j].velocity;
					}
					else if (enemies[j].direction == 3)
					{
						enemies[j].canMoveLeft = false;
						enemies[j].direction = net::generateRandom(4);
						enemies[j].temp_x += enemies[j].velocity;
					}
					else if (enemies[j].direction == 4)
					{
						enemies[j].canMoveRight = false;
						enemies[j].direction = net::generateRandom(4);
						enemies[j].temp_x -= enemies[j].velocity;
					}
				}
			}

			// projectile - wall collision
			for (int j = 0; j < projectiles.size(); j++)
			{
				if (net::detectCollision(walls[i], projectiles[j]))
				{


					projectiles[j].isAlive = false;
					if (walls[i].destructible == true)
					{
						walls[i].hp -= projectiles[j].attackDamage;
					}

					if (walls[i].hp <= 0)
					{
						walls[i].isAlive = false;
					}
				}
			}

			if (walls[i].isAlive == false)
			{
				walls.erase(walls.begin() + i);
			}
		}

		// projectile
		for (int i = 0; i < projectiles.size(); i++)
		{
			projectiles[i].update();

			// projectile - enemy collision
			for (int j = 0; j < enemies.size(); j++)
			{
				if (net::detectCollision(enemies[j], projectiles[i]))
				{
					projectiles[i].isAlive = false;
					enemies[j].aggroedBy = projectiles[i].id;
					enemies[j].hp -= projectiles[i].attackDamage;

					if (enemies[j].hp <= 0)
					{
						enemies[j].isAlive = false;
						enemies[j].aggroedBy = 0;
					}
				}
			}

			//// projectile - players collision
			//for (int j = 0; j < players.size(); j++)
			//{
			//	if (net::detectCollision(projectiles[i], players[j]))
			//	{
			//		projectiles[i].isAlive = false;
			//		players[j].hp -= projectiles[i].attackDamage;

			//		if (players[j].hp <= 0)
			//		{
			//			players[j].isAlive = false;
			//		}
			//	}
			//}



			if (projectiles[i].isAlive == false)
			{
				projectiles.erase(projectiles.begin() + i);
			}
		}

		// enemyMonster
		for (int i = 0; i < enemies.size(); i++)
		{
			enemies[i].update();

			// enemyMonster - player collision
			for (int j = 0; j < players.size(); j++)
			{
				if (abs(players[j].collisionRect_y - enemies[i].collisionRect_y) <= 100 &&
					abs(players[j].collisionRect_x - enemies[i].collisionRect_x) <= 100)
				{
					enemies[i].aggroedBy = players[j].id;
				}
				else
				{
					enemies[i].aggroedBy = 0;
				}

				if (net::detectCollision(players[j], enemies[i]))
				{
					players[j].hp -= enemies[i].attackDamage;

					if (players[j].powerUpLevel > 1)
					{
						players[j].powerUpLevel--;
					}
				}
			}

			// AI
			for (int j = 0; j < players.size(); j++)
			{

				if (enemies[i].aggroedBy == players[j].id)
				{
					if (players[j].collisionRect_x < enemies[i].collisionRect_x &&
						players[j].collisionRect_y - enemies[i].collisionRect_y <= 40)
					{
						enemies[i].direction = 3;
					}
					if (players[j].collisionRect_x > enemies[i].collisionRect_x &&
						players[j].collisionRect_y - enemies[i].collisionRect_y <= 40)
					{
						enemies[i].direction = 4;
					}
					if (players[j].collisionRect_y < enemies[i].collisionRect_y &&
						players[j].collisionRect_x - enemies[i].collisionRect_x <= 40)
					{
						enemies[i].direction = 1;
					}
					if (players[j].collisionRect_y > enemies[i].collisionRect_y &&
						players[j].collisionRect_x - enemies[i].collisionRect_x <= 80)
					{
						enemies[i].direction = 2;
					}
				}
			}




			if (enemies[i].isAlive == false)
			{
				// generate item           
				item.isAlive = true;
				int random = net::generateRandom(2);

				if (random == 1)
				{
					item.collisionRect_x = enemies[i].collisionRect_x;
					item.collisionRect_y = enemies[i].collisionRect_y;
					item.type = COIN;
					item.scaleValue = item.type == COIN ? 0.2 : 0.4;
					item.id = item_id++;
					items.push_back(item);
				}
				else if (random == 2)
				{
					item.collisionRect_x = enemies[i].collisionRect_x;
					item.collisionRect_y = enemies[i].collisionRect_y;
					item.type = POWERUP;
					item.scaleValue = item.type == COIN ? 0.2 : 0.4;
					item.id = item_id++;
					items.push_back(item);
				}
				enemies.erase(enemies.begin() + i);
			}
		}


		boost::asio::streambuf buf;
		net::ServerPacket pack;

		if (players.size() > 0)
			pack.players = players;
		if (projectiles.size() > 0)
			pack.projectiles = projectiles;
		if (enemies.size() > 0)
			pack.enemies = enemies;
		if (items.size() > 0)
			pack.items = items;
		if (walls.size() > 0)
			pack.walls = walls;

		std::cout << "[INFO] player size : " << players.size() << std::endl;
		std::cout << "[INFO] projectile size : " << projectiles.size() << std::endl;
		std::cout << "[INFO] enemy size : " << enemies.size() << std::endl;
		std::cout << "[INFO] wall size : " << walls.size() << std::endl;
		std::cout << "[INFO] item size : " << items.size() << std::endl;


		std::ostream oss(&buf);

		pack.save(oss);
		boost::asio::streambuf::const_buffers_type bufs = buf.data();
		std::string str(boost::asio::buffers_begin(bufs),
			boost::asio::buffers_begin(bufs) + bufs.size());
		m_response = str;//"response...";
		m_response += "\r\n";
		for (int i = 0; i < clients.size(); i++)
		{
			std::shared_ptr<boost::asio::ip::tcp::socket> client = clients[i];
			boost::asio::async_write(*client.get(),
				boost::asio::buffer(m_response),
				[this, client](const boost::system::error_code& ec,
					std::size_t bytes_transferred)
				{

					if (ec)
					{
						std::cout << "async_write Error : " << ec.message() << std::endl;
					}
					else
					{
						try
						{
							//std::cout << "Sent : " << bytes_transferred << std::endl;

							if (clients.size() > 2)
							{
								for (std::vector<std::shared_ptr<boost::asio::ip::tcp::socket>>::iterator iter = clients.begin(); iter != clients.end(); ++iter)
								{
									if (*iter == client)
									{
										clients.erase(iter);
										break;
									}
								}
								onResponseSent(ec, bytes_transferred);
							}
						}
						catch (const std::exception& e)
						{

						}
					}

				});
		}

	}
private:
	void onRequestReceived(const boost::system::error_code& ec,
		std::size_t bytes_transferred)
	{
		if (ec)
		{
			std::cout << "onRequestReceived Error : " << ec.message() << std::endl;

			onFinish();
			return;
		}

		m_response = ProcessRequest(m_request);

		if (m_response == "connected")
		{
			m_response = std::to_string(id);
			m_response += "\r\n";


			net::Player newPlayer = net::Player();
			newPlayer.id = id++;
			newPlayer.isAlive = true;
			newPlayer.port = m_sock->remote_endpoint().port();

			if (players.size() == 0)
				players.push_back(newPlayer);
			else
			{

				try
				{
					bool found = false;
					for (int i = 0; i < players.size(); i++)
					{
						players.at(i);

						if (newPlayer.id == players[i].id)
						{
							found = true;

						}
					}
					if (found == false)
						players.push_back(newPlayer);
				}
				catch (int e)
				{

				}
			}



			boost::asio::async_write(*m_sock.get(),
				boost::asio::buffer(m_response),
				[this](const boost::system::error_code& ec,
					std::size_t bytes_transferred)
				{
					if (ec)
					{
						std::cout << "Error..." << std::endl;
					}
					else
					{
						//std::cout << "Sent" << std::endl;
						onResponseSent(ec, bytes_transferred);
					}
				});
		}
		else
		{

		}

	}

	void onResponseSent(const boost::system::error_code& ec, std::size_t bytes_transferred)
	{

		if (ec)
		{
			std::cout << "onResponseSent error: " << ec.message() << std::endl;
		}

		onFinish();
	}

	void onFinish()
	{
		//delete this;
	}

	std::string ProcessRequest(boost::asio::streambuf& request)
	{
		// do something

		boost::asio::streambuf::const_buffers_type bufs = request.data();
		std::string str(boost::asio::buffers_begin(bufs),
			boost::asio::buffers_begin(bufs) + bufs.size());
		std::cout << "[CLIENT REQUEST] Request : " << str << std::endl;
		std::string response;
		if (str == "connect")
		{
			response = "connected";
			//std::cout << response << std::endl;
		}
		else if (str.find("id_") != std::string::npos)
		{
			response = "ping";
			//std::cout << "Response :: " << response << std::endl;

			size_t i = 0;
			for (; i < str.length(); i++) { if (isdigit(str[i])) break; }

			// remove the first chars, which aren't digits
			str = str.substr(i, str.length() - i);

			// convert the remaining text to an integer
			int id = atoi(str.c_str());

			std::cout << "[INFO] connectino with player - " << id << " alive" << std::endl;

			for (int i = 0; i < players.size(); i++)
			{
				if (players[i].id == id)
				{
					players[i].isConnected = true;
					players[i].connectCounter = 1;
				}
			}
			std::cout << m_sock->remote_endpoint().port() << std::endl;
		}
		else // process key input
		{
			net::ClientPacket p;
			p.load(str);
			//std::cout << "Request2 : " << p.key << " " << p.player_id << std::endl;

			if (p.key >= 1 && p.key <= 4)
			{
				for (int i = 0; i < players.size(); i++)
				{
					if (p.player_id == players[i].id)
					{
						players[i].direction = p.key;
						players[i].update();

						// players - wall collision
						for (int j = 0; j < walls.size(); j++)
						{
							if (net::detectCollision(players[i], walls[j]))
							{
								if (players[i].direction == 1)
								{
									players[i].canMoveUp = false;
									players[i].temp_y += players[i].velocity;
								}
								else if (players[i].direction == 2)
								{
									players[i].canMoveDown = false;
									players[i].temp_y -= players[i].velocity;
								}
								else if (players[i].direction == 3)
								{
									players[i].canMoveLeft = false;
									players[i].temp_x += players[i].velocity;
								}
								else if (players[i].direction == 4)
								{
									players[i].canMoveRight = false;
									players[i].temp_x -= players[i].velocity;
								}
							}
						}
						break;
					}
				}
			}
			else if (p.key == 5)
			{

				int random = net::generateRandom(3);
				projectile.id = p.player_id;
				projectile.isAlive = true;
				int counter = 0;

				for (int i = 0; i < players.size(); i++)
				{
					if (p.player_id == players[i].id)
					{
						projectile.direction = players[i].direction;
						projectile.temp_projectile_x = projectile.collisionRect_x = players[i].collisionRect_x;
						projectile.temp_projectile_y = projectile.collisionRect_y = players[i].collisionRect_y;


						while (counter < players[i].powerUpLevel)
						{
							projectile.temp_projectile_x = projectile.collisionRect_x += counter * random;
							projectile.temp_projectile_y = projectile.collisionRect_y += counter * random;
							counter++;
							projectile.id = projectile_id++;
							projectiles.push_back(projectile);
						}


						projectile.isAlive = false;
						break;
					}
				}
			}

			response = str;
		}

		return response;
	}

private:
	std::shared_ptr<boost::asio::ip::tcp::socket> m_sock;
	std::string m_response;

	boost::asio::streambuf m_request;
};

class Acceptor
{
public:
	Acceptor(boost::asio::io_context& io, unsigned short port_num) :
		m_io(io),
		m_acceptor(m_io, boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::any(), port_num)),
		m_isStopped(false)
	{

	}

	void Start()
	{
		m_acceptor.listen();
		InitAccept();

	}

	void Stop()
	{
		m_isStopped.store(true);
	}

private:
	void InitAccept()
	{
		std::shared_ptr<boost::asio::ip::tcp::socket> sock(new boost::asio::ip::tcp::socket(m_io));

		m_acceptor.async_accept(*sock.get(),
			[this, sock](const boost::system::error_code& error)
			{
				//boost::asio::socket_base::keep_alive option(true);
				/*boost::asio::ip::tcp::no_delay option(true);
				sock->set_option(option);*/
				clients.push_back(sock);
				onAccept(error, sock);
			});
	}

	void onAccept(const boost::system::error_code& ec, std::shared_ptr<boost::asio::ip::tcp::socket> sock)
	{
		if (ec)
		{
			std::cout << "onAccept Error : " << ec.message() << std::endl;
		}
		else
		{

			(new Service(sock))->StartHandling();
			//(new Service(sock))->StartHandling2(); // tcp game world send

			if (!m_isStopped.load())
			{
				InitAccept();
			}

		}
	}
private:
	boost::asio::io_context& m_io;

	boost::asio::ip::tcp::acceptor m_acceptor;
	std::atomic<bool> m_isStopped;
};

class Server
{
public:
	Server()
	{
		m_work.reset(new boost::asio::io_context::work(m_io));
	}

	void Start(unsigned short port_num, unsigned int thread_pool_size)
	{
		assert(thread_pool_size > 0);

		acc.reset(new Acceptor(m_io, port_num));
		acc->Start();

		for (std::size_t i = 0; i < thread_pool_size - 1; i++)
		{
			std::unique_ptr<std::thread> th(new std::thread([this]()
				{
					m_io.run();
				}));
			m_thread_pool.push_back(std::move(th));
		}
	}

	void StartUDP(unsigned short port_num)
	{
		std::unique_ptr<std::thread> th(new std::thread([this]()
			{
				sender s(m_io_udp, boost::asio::ip::address::from_string("239.255.0.1"));

				m_io_udp.run();
			}));
		m_thread_pool.push_back(std::move(th));
	}

	void Stop()
	{
		acc->Stop();
		m_io.stop();

		for (auto& th : m_thread_pool)
		{
			th->join();
		}
	}

	void createRoomWall()
	{
		wall.isAlive = true;

		counter = 0;
		while (counter < roomSize)
		{
			// first room
			wall.collisionRect_x = 50 * counter + initialRoomX;
			wall.collisionRect_y = initialRoomY;
			wall.destructible = net::generateRandomBool();
			wall.id = wall_id++;
			walls.push_back(wall);
			wall.collisionRect_x = 50 * counter + initialRoomX;
			wall.collisionRect_y = 50 * roomSize + initialRoomY;
			wall.destructible = net::generateRandomBool();
			wall.id = wall_id++;
			walls.push_back(wall);
			wall.collisionRect_x = initialRoomX;
			wall.collisionRect_y = 50 * counter + initialRoomY;
			wall.destructible = net::generateRandomBool();
			wall.id = wall_id++;
			walls.push_back(wall);
			wall.collisionRect_x = 50 * roomSize + initialRoomX;
			wall.collisionRect_y = 50 * counter + initialRoomY;
			wall.destructible = net::generateRandomBool();
			wall.id = wall_id++;
			walls.push_back(wall);

			// second room
			wall.collisionRect_x = 50 * counter + initialRoomX + (roomSize * 50) + 50;
			wall.collisionRect_y = initialRoomY;
			wall.destructible = net::generateRandomBool();
			wall.id = wall_id++;
			walls.push_back(wall);
			wall.collisionRect_x = 50 * counter + initialRoomX + (roomSize * 50) + 50;
			wall.collisionRect_y = 50 * roomSize + initialRoomY;
			wall.destructible = net::generateRandomBool();
			wall.id = wall_id++;
			walls.push_back(wall);
			wall.collisionRect_x = initialRoomX + (roomSize * 50) + 50;
			wall.collisionRect_y = 50 * counter + initialRoomY;
			wall.destructible = net::generateRandomBool();
			wall.id = wall_id++;
			walls.push_back(wall);
			wall.collisionRect_x = 50 * roomSize + initialRoomX + (roomSize * 50) + 50;
			wall.collisionRect_y = 50 * counter + initialRoomY;
			wall.destructible = net::generateRandomBool();
			wall.id = wall_id++;
			walls.push_back(wall);

			//// third room
			//wall.collisionRect_x = 50 * counter + initialRoomX + (roomSize * 50 * 2) + 50;
			//wall.collisionRect_x = initialRoomY;
			//wall.destructible = net::generateRandomBool();
			////wall.sprite.setTextureRect(wall.destructible ? sf::IntRect(0, 0, 18, 18) : sf::IntRect(0, 32, 18, 18));
			//walls.push_back(wall);
			//wall.collisionRect_x = 50 * counter + initialRoomX + (roomSize * 50 * 2) + 50;
			//wall.collisionRect_y = 50 * roomSize + initialRoomY;
			//wall.destructible = net::generateRandomBool();
			////wall.sprite.setTextureRect(wall.destructible ? sf::IntRect(0, 0, 18, 18) : sf::IntRect(0, 32, 18, 18));
			//walls.push_back(wall);
			//wall.collisionRect_x = initialRoomX + (roomSize * 50 * 2) + 50;
			//wall.collisionRect_y = 50 * counter + initialRoomY;
			//wall.destructible = net::generateRandomBool();
			////wall.sprite.setTextureRect(wall.destructible ? sf::IntRect(0, 0, 18, 18) : sf::IntRect(0, 32, 18, 18));
			//walls.push_back(wall);
			//wall.collisionRect_x = 50 * roomSize + initialRoomX + (roomSize * 50 * 2) + 50;
			//wall.collisionRect_y = 50 * counter + initialRoomY;
			//wall.destructible = net::generateRandomBool();
			////wall.sprite.setTextureRect(wall.destructible ? sf::IntRect(0, 0, 18, 18) : sf::IntRect(0, 32, 18, 18));
			//walls.push_back(wall);

			counter++;
		}

		counter = 0;
		while (counter < bossRoomSize)
		{
			wall.collisionRect_x = 50 * counter + initialRoomX + (bossRoomSize * 50 * 2) + 50;
			wall.collisionRect_y = initialRoomY;
			wall.destructible = net::generateRandomBool();
			wall.id = wall_id++;
			walls.push_back(wall);
			wall.collisionRect_x = 50 * counter + initialRoomX + (bossRoomSize * 50 * 2) + 50;
			wall.collisionRect_y = 50 * bossRoomSize + initialRoomY;
			wall.destructible = net::generateRandomBool();
			wall.id = wall_id++;
			walls.push_back(wall);
			wall.collisionRect_x = initialRoomX + (bossRoomSize * 50 * 2) + 50;
			wall.collisionRect_y = 50 * counter + initialRoomY;
			wall.destructible = net::generateRandomBool();
			wall.id = wall_id++;
			walls.push_back(wall);
			wall.collisionRect_x = 50 * bossRoomSize + initialRoomX + (bossRoomSize * 50 * 2) + 50;
			wall.collisionRect_y = 50 * counter + initialRoomY;
			wall.destructible = net::generateRandomBool();
			wall.id = wall_id++;
			walls.push_back(wall);
			counter++;
		}
	}

	void createShopItem()
	{
		item.isAlive = true;
		item.inShop = true;
		item.cost = 10;
		item.collisionRect_x = (50 * roomSize / 2 + initialRoomX + (roomSize * 50 * 2) + 50);
		item.collisionRect_y = 50 * roomSize / 2 + initialRoomY;
		item.type = POWERUP;
		item.scaleValue = item.type == COIN ? 0.2 : 0.4;
		item.id = item_id++;
		items.push_back(item);
		item.inShop = false;
	}

	void createEnemyandItemWall()
	{
		enemy.isAlive = true;
		wall.isAlive = true;

		// create enemy and wall in room
		counter = 1;
		while (counter < roomSize - 2)
		{
			counter2 = 1;
			while (counter2 < roomSize - 2)
			{
				if (counter != counter2)
				{
					int tempRandom = net::generateRandom(7);

					if (tempRandom == 1)
					{
						//// first room
						wall.destructible = true;
						wall.collisionRect_x = (counter * 50) + 100 + initialRoomX;
						wall.collisionRect_y = (counter2 * 50) + 100 + initialRoomY;
						wall.id = wall_id++;
						walls.push_back(wall);

						//// second room
						wall.destructible = true;
						wall.collisionRect_x = 50 * counter + initialRoomX + (roomSize * 50) + 150;
						wall.collisionRect_y = (counter2 * 50) + 100 + initialRoomY;
						wall.id = wall_id++;
						walls.push_back(wall);
					}
					else if (tempRandom == 2)
					{
						// first room
						enemy.temp_x = enemy.collisionRect_x = (counter * 50) + 100 + initialRoomX;
						enemy.temp_y = enemy.collisionRect_y = (counter2 * 50) + 100 + initialRoomY;
						enemy.id = enemy_id++;
						enemies.push_back(enemy);


						//// second room
						enemy.temp_x = enemy.collisionRect_x = 50 * counter + initialRoomX + (roomSize * 50) + 150;
						enemy.temp_y = enemy.collisionRect_y = (counter2 * 50) + 100 + initialRoomY;
						enemy.id = enemy_id++;
						enemies.push_back(enemy);
					}
				}
				counter2++;
			}
			counter++;
		}

		// boss room enemies
		counter = 0;
		while (counter < bossRoomSize - 1)
		{
			counter2 = 0;
			while (counter2 < bossRoomSize - 1)
			{
				int tempRandom = net::generateRandom(40);

				if (tempRandom == 1)
				{
					wall.destructible = true;
					wall.collisionRect_x = 50 * counter + initialRoomX + (bossRoomSize * 50 * 2) + 100;
					wall.collisionRect_y = (counter2 * 50) + 50 + initialRoomY;
					wall.id = wall_id++;
					walls.push_back(wall);
				}
				else if (tempRandom == 2)
				{

					enemy.temp_x = enemy.collisionRect_x = 50 * counter + initialRoomX + (bossRoomSize * 50 * 2) + 100;
					enemy.temp_y = enemy.collisionRect_y = (counter2 * 50) + 50 + initialRoomY;
					enemy.id = enemy_id++;
					enemies.push_back(enemy);
				}
				counter2++;
			}
			counter++;
		}
	}

	void createBoss()
	{
		enemy.isAlive = true;
		enemy.maxHp = 100;
		enemy.hp = 100;
		enemy.isBoss = true;
		enemy.attackDamage = 10;
		enemy.temp_x = enemy.collisionRect_x = 50 * bossRoomSize / 2 + initialRoomX + (bossRoomSize * 50 * 2) + 100;
		enemy.temp_y = enemy.collisionRect_y = (bossRoomSize / 2 * 50) + 50 + initialRoomY;
		enemy.id = enemy_id++;
		enemies.push_back(enemy);
		enemy.isBoss = false;
	}

private:
	boost::asio::io_context m_io;
	boost::asio::io_context m_io_udp;
	std::unique_ptr<boost::asio::io_context::work> m_work;
	std::unique_ptr<Acceptor> acc;
	std::vector<std::unique_ptr<std::thread>> m_thread_pool;

	int roomSize = 10;
	int bossRoomSize = 15;
	int verticalDoorAt = 2;
	int horizontalDoorAt = 2;
	int initialRoomX = 200;
	int initialRoomY = 300;
};

const unsigned int DEFAULT_THREAD_POOL_SIZE = 2;

int main()
{
	unsigned short port_num = 5555;
	unsigned short udp_port_num = 30001;

	try
	{
		Server server;

		unsigned int thread_pool_size = std::thread::hardware_concurrency() * 2;

		if (thread_pool_size == 0)
		{
			thread_pool_size = DEFAULT_THREAD_POOL_SIZE;
		}
		server.createRoomWall();
		server.createEnemyandItemWall();
		server.createShopItem();
		server.createBoss();

		server.Start(port_num, thread_pool_size);

		server.StartUDP(udp_port_num);
		std::cout << "Server is running..." << std::endl;
		while (true)
		{

			// ping counter -> disconnect
			for (int i = 0; i < players.size(); i++)
			{
				if (players[i].connectCounter++ > players[i].connectDelay)
				{
					players[i].connectCounter = 1;

					if (players[i].isConnected == false)
					{
						// disconnect
						std::cout << "[INFO] " << players[i].id << " - disconnected <<" << std::endl;
						players.erase(players.begin() + i);

						break;
					}

					if (players[i].connectCounter >= 1)
					{
						players[i].isConnected = false;
					}
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(33));
		}

		std::this_thread::sleep_for(std::chrono::seconds(36000));

		server.Stop();
	}
	catch (boost::system::system_error& e)
	{
		std::cout << "Error : " << e.what() << std::endl;
	}

	return 0;
}