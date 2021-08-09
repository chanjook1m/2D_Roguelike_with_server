#pragma once
#pragma once
#include <iostream>


#include <boost/serialization/serialization.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/shared_ptr.hpp>

namespace net
{
	class Projectile
	{
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& id;
			/*ar& velocity;
			ar& powerUpLevel;
			ar& direction;
			ar& hp;
			ar& score;
			ar& canMoveUp;
			ar& canMoveDown;
			ar& canMoveLeft;
			ar& canMoveRight;*/
		}
	public:
		int velocity = 2;
		float attackDamage = 0.f;
		int direction = 0; // 1 = up, 2 = down, 3 = left, 4 = right
		int lifetime = 100;
		int lifetimeCounter = 0;
		int id = 0;
	};

	class Player
	{
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& id;
			ar& collisionRect_x;
			ar& collisionRect_y;
			ar& velocity;
			ar& powerUpLevel;
			ar& direction;
			ar& hp;
			ar& maxHp;
			ar& score;
			ar& canMoveUp;
			ar& canMoveDown;
			ar& canMoveLeft;
			ar& canMoveRight;
			ar& moved;
		}
	public:
		int id = 1;
		int x = 0;
		int y = 0;
		int velocity = 2;
		float attackDamage = 1.0f;
		int powerUpLevel = 1;
		int maxPowerUpLevel = 4;
		int walkSpriteNumber = 0;
		int direction = 0; // 1 = up, 2 = down, 3 = left, 4 = right
		int hp = 100;
		int maxHp = 100;
		int score = 0;

		bool canMoveUp = true;
		bool canMoveDown = true;
		bool canMoveLeft = true;
		bool canMoveRight = true;

		int collisionRect_x = 300;
		int collisionRect_y = 400;

		int temp_x = collisionRect_x;
		int temp_y = collisionRect_y;


		// projectile
		int projectile_x = collisionRect_x;
		int projectile_y = collisionRect_y;

		int temp_projectile_x = collisionRect_x;
		int temp_projectile_y = collisionRect_y;

		int lifetime = 30;
		int lifetimeCounter = 0;

		bool projectileAlive = false;

		bool updated = false;
		bool shooted = false;
		bool moved = false;
		int projectileDirection;
		void update()
		{
			move();
			//sprite.setPosition(collisionRect.getPosition());
		}

		void projectileUpdate()
		{
			if (moved && projectileAlive == false)
			{
				temp_projectile_x = collisionRect_x;
				temp_projectile_y = collisionRect_y;
			}

			if (projectileAlive)
			{
				if (lifetimeCounter == 0)
				{
					projectileDirection = direction;
				}

				if (projectileDirection == 1)
				{
					projectile_x = temp_projectile_x;
					projectile_y = temp_projectile_y - velocity;
					temp_projectile_y = projectile_y;
				}

				if (projectileDirection == 2 || projectileDirection == 0)
				{
					projectile_x = temp_projectile_x;
					projectile_y = temp_projectile_y + velocity;
					temp_projectile_y = projectile_y;
				}

				if (projectileDirection == 3)
				{
					projectile_y = temp_projectile_y;
					projectile_x = temp_projectile_x - velocity;
					temp_projectile_x = projectile_x;

				}

				if (projectileDirection == 4)
				{
					projectile_y = temp_projectile_y;
					projectile_x = temp_projectile_x + velocity;
					temp_projectile_x = projectile_x;
				}

				lifetimeCounter++;

				if (lifetimeCounter >= lifetime)
				{
					lifetimeCounter = 0;
					projectileAlive = false;
					shooted = false;
					temp_projectile_x = collisionRect_x;
					temp_projectile_y = collisionRect_y;

				}
			}
		}

		void move()
		{
			if (direction == 1 && canMoveUp)
			{
				//collisionRect.move(0.f, -velocity);
				//collisionRect_x = collisionRect.getPosition().x;
				//collisionRect_y = collisionRect.getPosition().y;
				collisionRect_x = temp_x;
				collisionRect_y = temp_y - velocity;
				temp_y = collisionRect_y;

				moved = true;
			}
			else if (direction == 2 && canMoveDown)
			{
				/*collisionRect.move(0.f, velocity);
				collisionRect_x = collisionRect.getPosition().x;
				collisionRect_y = collisionRect.getPosition().y;*/
				collisionRect_x = temp_x;
				collisionRect_y = temp_y + velocity;
				temp_y = collisionRect_y;

				moved = true;
			}
			else if (direction == 3 && canMoveLeft)
			{
				//collisionRect.move(-velocity, 0.f);
				collisionRect_x = temp_x - velocity;
				temp_x = collisionRect_x;
				collisionRect_y = temp_y;
				//collisionRect_y = collisionRect.getPosition().y;
				//std::cout << collisionRect_x << " and " << collisionRect_y << std::endl;
				moved = true;
			}
			else if (direction == 4 && canMoveRight)
			{
				//collisionRect.move(velocity, 0.f);
				collisionRect_x = temp_x + velocity;
				temp_x = collisionRect_x;
				collisionRect_y = temp_y;
				//collisionRect_y = collisionRect.getPosition().y;
				//std::cout << collisionRect_x << " and " << collisionRect_y << std::endl;
				moved = true;
			}
			else
			{
				canMoveUp = true;
				canMoveDown = true;
				canMoveLeft = true;
				canMoveRight = true;
				collisionRect_x = temp_x;
				collisionRect_y = temp_y;
				moved = false;
			}
			//direction = 0;
		}
	};

	// ------------------------------

	class LoginPacket
	{
	public:
		int type;
		int id;

	private:

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& type;
			ar& id;

		}

	public:



		void save(std::ostream& oss)
		{
			boost::archive::binary_oarchive oa(oss);
			oa&* (this);
		}
		void load(std::string str_data)
		{
			std::istringstream iss(str_data);
			boost::archive::binary_iarchive ia(iss);
			ia&* (this);
		}
	};

	class ServerPacket
	{
	public:
		int type;
		std::vector<Player> players;
		std::vector<Projectile> projectiles;
		std::string msg;
		int id;

	private:

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& type;
			ar& id;
			ar& msg;
			ar& players;
			ar& projectiles;

		}

	public:
		ServerPacket()
		{

		}

		ServerPacket(std::string msg) : msg(msg)
		{

		}
		void save(std::ostream& oss)
		{
			boost::archive::binary_oarchive oa(oss);
			oa&* (this);
		}
		void load(std::string str_data)
		{
			std::istringstream iss(str_data);
			boost::archive::binary_iarchive ia(iss);
			ia&* (this);
		}
	};

	class ClientPacket
	{
	public:
		int key; // 1 = up, ... , 5 = space
		int player_id;

	private:

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& key;
			ar& player_id;
		}

	public:
		ClientPacket()
		{

		}
		ClientPacket(int key, int player_id) : key(key), player_id(player_id)
		{

		}

		void save(std::ostream& oss)
		{
			boost::archive::binary_oarchive oa(oss);
			oa&* (this);
		}
		void load(std::string str_data)
		{
			std::istringstream iss(str_data);
			boost::archive::binary_iarchive ia(iss);
			ia&* (this);
		}
	};
}