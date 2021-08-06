#include <iostream>
#include <ctime>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include "net.hpp"



int main()
{
	static int id = 0;
	std::cout << "Server running" << std::endl;

	sf::TcpListener listener;
	listener.setBlocking(false);
	sf::SocketSelector selector;
	bool done = false;

	std::vector<sf::TcpSocket*> clients;
	std::vector<net::Player> players;

	listener.listen(5555);
	selector.add(listener);

	while (!done)
	{
		if (selector.wait(sf::seconds(0.09f)))
		{
			if (selector.isReady(listener))
			{
				sf::TcpSocket* socket = new sf::TcpSocket();

				listener.accept(*socket);

				sf::Packet packet;



				if (socket->receive(packet) == sf::Socket::Done)
				{
					if (packet.getDataSize() > 0)
					{
						std::cout << "packet data: " << packet.getData() << std::endl;
						std::cout << id << " has connected" << std::endl;
						clients.push_back(socket);
						selector.add(*socket);

						sf::Packet packet;
						net::Player newPlayer = net::Player();
						newPlayer.id = id++;
						players.push_back(newPlayer);
						//newPlayer.hp = 80;
						packet << newPlayer;

						socket->send(packet);
					}

				}


			}
			else
			{

				/*while (true)
				{*/

				//}
				for (int i = 0; i < clients.size(); i++)
				{
					if (selector.isReady(*clients[i]))
					{

						sf::Packet packet, sendPacket;
						if (clients[i]->receive(packet) == sf::Socket::Done)
						{
							packet >> players[i];
							std::cout << "id: " << players[i].id << " - pos :" << players[i].collisionRect_x << " , " << players[i].collisionRect_y << std::endl;
							players[i].updated = true;
							/*players[i].update();
							sendPacket << players[i];
							clients[i]->send(sendPacket);*/
							/*std::string text;
							packet >> text;
							sendPacket << text;
							for (int j = 0; j < clients.size(); j++)
							{
								if (i != j)
								{
									clients[j]->send(sendPacket);
								}
							}*/
						}
					}
				}
			}
		}
		/*else
		{*/
		//std::cout << players.size() << " and " << clients.size() << std::endl;
		//double time = GetTickCount();
		// UpdateGame();


		for (size_t i = 0; i < players.size(); i++)
		{
			//if (players[i].shooted)
			//{
			std::cout << "shoot" << std::endl;
			players[i].projectileUpdate();
			//}
			if (players[i].updated)
			{

				players[i].update();

				players[i].updated = false;
			}
		}

		for (size_t i = 0; i < clients.size(); i++)
		{


			sf::TcpSocket* client = clients[i];
			client->setBlocking(false);
			for (size_t j = 0; j < players.size(); j++)
			{
				sf::Packet packet;

				packet << players[j];
				if (client->send(packet) != sf::Socket::Done) {
					std::cout << "[SERVER] could not send packet" << std::endl;
				}
				else
				{
					std::cout << "[SERVER] send to player " << players[j].id << std::endl;
				}
			}
		}
		//
		//double deltaT = GetTickCount() - time;
		//if (deltaT < 33)
		//{
		//	//Sleep(33 - deltaT);
		//}
	//}
	}
	for (std::vector<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); it++)
		delete* it;

	return 0;
}