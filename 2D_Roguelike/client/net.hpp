#pragma once
#include <iostream>


#include <boost/serialization/serialization.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/base_object.hpp>
#include <stdlib.h>

namespace net
{
	bool generateRandomBool()
	{
		int randomNumber = rand();
		int random = (randomNumber % 2) + 1;

		if (random == 1)
			return true;
		else
			return false;
	}

	int generateRandom(int max)
	{
		int randomNumber = rand();
		int random = (randomNumber % max) + 1;

		return random;
	}

	template <typename T1, typename T2>
	bool detectCollision(T1 a, T2 b)
	{
		/*if (abs(a.collisionRect_x - (b.collisionRect_x + b.width)) < 50 &&
			abs((a.collisionRect_x + a.width) - (b.collisionRect_x)) < 50 &&
			abs(a.collisionRect_y - (b.collisionRect_y + b.height)) < 30 &&
			abs((a.collisionRect_y - a.height) - (b.collisionRect_y)) < 30) {*/
		if (a.collisionRect_x< b.collisionRect_x + 3 * b.width &&
			a.collisionRect_x + a.width > b.collisionRect_x &&
			a.collisionRect_y < b.collisionRect_y + 3 * b.height &&
			a.collisionRect_y + a.height > b.collisionRect_y) {
			// collision detected!
			//std::cout << "ooooooooooooooooooooooooo detected " << std::endl;
			return true;
		}
		else
		{
			//std::cout << "xxxxxxxxxxxxxx not detected " << std::endl;
			return false;
		}
	}

	class Wall
	{
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& collisionRect_x;
			ar& collisionRect_y;
			ar& destructible;
			ar& hp;
			ar& width;
			ar& height;
			ar& isAlive;
			ar& isCollide;
			ar& id;
		}

	public:
		int id = 0;
		int collisionRect_x;
		int collisionRect_y;
		bool destructible = false;
		int hp = 3;
		int width = 18;
		int height = 18;
		bool isAlive = false;
		bool isCollide = false;
	};

	class Item
	{
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& animateSpriteNumber;
			ar& delayCounter;
			ar& animateDelay;
			ar& type;
			ar& scaleValue;
			ar& inShop;
			ar& cost;
			ar& collisionRect_x;
			ar& collisionRect_y;
			ar& isAlive;
			ar& isCollide;
			ar& id;
		}

		enum types
		{
			COIN = 1,
			POWERUP,
		};
	public:
		int id;
		bool isCollide = false;
		int animateSpriteNumber = 0;
		int delayCounter = 0;
		int animateDelay = 5;
		int type = 0;
		float scaleValue = 0.f;
		bool inShop = false;
		int cost = 0;
		int collisionRect_x;
		int collisionRect_y;
		bool isAlive = false;
		int width = 10;
		int height = 10;

		void update()
		{
			animate();
		}

		void animate()
		{
			delayCounter++;
			if (delayCounter == animateDelay)
			{
				delayCounter = 0;
				animateSpriteNumber++;
				if (animateSpriteNumber == 5) {
					animateSpriteNumber = 0;
				}
			}
		}
	};

	class Enemy
	{
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& isAlive;
			ar& velocity;
			ar& attackDamage;
			ar& walkSpriteNumber;
			ar& direction;
			ar& delayCounter;
			ar& movementDelay;
			ar& hp;
			ar& maxHp;

			ar& canMoveUp;
			ar& canMoveDown;
			ar& canMoveLeft;
			ar& canMoveRight;

			ar& aggroedBy;

			ar& collisionRect_x;
			ar& collisionRect_y;
			ar& isCollide;
			ar& id;
		}

	public:
		int id = 0;
		bool isCollide = false;
		bool isAlive = false;
		int velocity = 1;
		float attackDamage = 1.0f;
		int walkSpriteNumber = 0;
		int direction = 0;
		int delayCounter = 0;
		int movementDelay = 40;
		int hp = 3;
		int maxHp = 3;
		int width = 48;
		int height = 48;

		bool canMoveUp = true;
		bool canMoveDown = true;
		bool canMoveLeft = true;
		bool canMoveRight = true;

		int aggroedBy = 0;

		int collisionRect_x = 300;
		int collisionRect_y = 400;

		int temp_x = collisionRect_x;
		int temp_y = collisionRect_y;
		void move()
		{
			if (direction == 1 && canMoveUp)
			{
				//collisionRect.move(0.f, -velocity);
				//sprite.setTextureRect(sf::IntRect(walkSpriteNumber * spriteWidth + x, spriteHeight * 3 + y, spriteWidth, spriteHeight));
				collisionRect_x = temp_x;
				collisionRect_y = temp_y - velocity;
				temp_y = collisionRect_y;

				canMoveUp = true;
				canMoveDown = true;
				canMoveLeft = true;
				canMoveRight = true;
			}
			else if (direction == 2 && canMoveDown)
			{
				//collisionRect.move(0.f, velocity);
				//sprite.setTextureRect(sf::IntRect(walkSpriteNumber * spriteWidth + x, 0 + y, spriteWidth, spriteHeight));
				collisionRect_x = temp_x;
				collisionRect_y = temp_y + velocity;
				temp_y = collisionRect_y;

				canMoveUp = true;
				canMoveDown = true;
				canMoveLeft = true;
				canMoveRight = true;
			}
			else if (direction == 3 && canMoveLeft)
			{
				//collisionRect.move(-velocity, 0.f);
				//sprite.setTextureRect(sf::IntRect(walkSpriteNumber * spriteWidth + x, spriteHeight * 1 + y, spriteWidth, spriteHeight));
				collisionRect_x = temp_x - velocity;
				temp_x = collisionRect_x;
				collisionRect_y = temp_y;

				canMoveUp = true;
				canMoveDown = true;
				canMoveLeft = true;
				canMoveRight = true;
			}
			else if (direction == 4 && canMoveRight)
			{
				//collisionRect.move(velocity, 0.f);
				//sprite.setTextureRect(sf::IntRect(walkSpriteNumber * spriteWidth + x, spriteHeight * 2 + y, spriteWidth, spriteHeight));
				collisionRect_x = temp_x + velocity;
				temp_x = collisionRect_x;
				collisionRect_y = temp_y;

				canMoveUp = true;
				canMoveDown = true;
				canMoveLeft = true;
				canMoveRight = true;
			}
			else
			{
				canMoveUp = true;
				canMoveDown = true;
				canMoveLeft = true;
				canMoveRight = true;
			}




			walkSpriteNumber++;
			if (walkSpriteNumber == 3) {
				walkSpriteNumber = 0;
			}

			delayCounter++;
			if (delayCounter == movementDelay)
			{
				int randomNumber = rand();
				int random = (randomNumber % 4) + 1;
				delayCounter = 0;
				direction = random;
			}
		}
		void update()
		{
			move();
		}
	};

	class Projectile
	{
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& id;
			ar& velocity;
			ar& attackDamage;
			ar& direction;
			ar& lifetime;
			ar& lifetimeCounter;
			ar& collisionRect_x;
			ar& collisionRect_y;
			ar& isAlive;
			ar& isCollide;
		}
	public:
		bool isCollide = false;
		int velocity = 2;
		float attackDamage = 1.0f;
		int direction = 0; // 1 = up, 2 = down, 3 = left, 4 = right
		int lifetime = 100;
		int lifetimeCounter = 0;
		int id = 0;
		int collisionRect_x;
		int collisionRect_y;
		bool isAlive = false;
		int width = 8;
		int height = 8;

		int temp_projectile_x = collisionRect_x;
		int temp_projectile_y = collisionRect_y;

		void update()
		{
			if (direction == 1)
			{
				collisionRect_x = temp_projectile_x;
				collisionRect_y = temp_projectile_y - velocity;
				temp_projectile_y = collisionRect_y;
			}

			if (direction == 2 || direction == 0)
			{
				collisionRect_x = temp_projectile_x;
				collisionRect_y = temp_projectile_y + velocity;
				temp_projectile_y = collisionRect_y;
			}

			if (direction == 3)
			{
				collisionRect_y = temp_projectile_y;
				collisionRect_x = temp_projectile_x - velocity;
				temp_projectile_x = collisionRect_x;

			}

			if (direction == 4)
			{
				collisionRect_y = temp_projectile_y;
				collisionRect_x = temp_projectile_x + velocity;
				temp_projectile_x = collisionRect_x;
			}

			lifetimeCounter++;

			if (lifetimeCounter >= lifetime)
			{
				lifetimeCounter = 0;
				temp_projectile_x = collisionRect_x;
				temp_projectile_y = collisionRect_y;
				isAlive = false;
			}
		}
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
			ar& isAlive;
			ar& isCollide;
		}
	public:
		bool isCollide = false;
		bool isAlive = false;
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
		int width = 24;
		int height = 32;

		bool canMoveUp = true;
		bool canMoveDown = true;
		bool canMoveLeft = true;
		bool canMoveRight = true;

		int collisionRect_x = 300;
		int collisionRect_y = 400;

		int temp_x = collisionRect_x;
		int temp_y = collisionRect_y;


		void update()
		{
			move();
			//sprite.setPosition(collisionRect.getPosition());
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
				canMoveUp = true;
				canMoveDown = true;
				canMoveLeft = true;
				canMoveRight = true;


			}
			else if (direction == 2 && canMoveDown)
			{
				/*collisionRect.move(0.f, velocity);
				collisionRect_x = collisionRect.getPosition().x;
				collisionRect_y = collisionRect.getPosition().y;*/
				collisionRect_x = temp_x;
				collisionRect_y = temp_y + velocity;
				temp_y = collisionRect_y;
				canMoveUp = true;
				canMoveDown = true;
				canMoveLeft = true;
				canMoveRight = true;


			}
			else if (direction == 3 && canMoveLeft)
			{
				//collisionRect.move(-velocity, 0.f);
				collisionRect_x = temp_x - velocity;
				temp_x = collisionRect_x;
				collisionRect_y = temp_y;
				canMoveUp = true;
				canMoveDown = true;
				canMoveLeft = true;
				canMoveRight = true;
				//collisionRect_y = collisionRect.getPosition().y;
				//std::cout << collisionRect_x << " and " << collisionRect_y << std::endl;

			}
			else if (direction == 4 && canMoveRight)
			{
				//collisionRect.move(velocity, 0.f);
				collisionRect_x = temp_x + velocity;
				temp_x = collisionRect_x;
				collisionRect_y = temp_y;
				canMoveUp = true;
				canMoveDown = true;
				canMoveLeft = true;
				canMoveRight = true;
				//collisionRect_y = collisionRect.getPosition().y;
				//std::cout << collisionRect_x << " and " << collisionRect_y << std::endl;

			}
			else
			{
				canMoveUp = true;
				canMoveDown = true;
				canMoveLeft = true;
				canMoveRight = true;
				collisionRect_x = temp_x;
				collisionRect_y = temp_y;

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
		std::vector<Enemy> enemies;
		std::vector<Item> items;
		std::vector<Wall> walls;
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
			ar& enemies;
			ar& walls;
			ar& items;
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