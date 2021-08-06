#pragma once

#include <SFML/Network.hpp>
#include <SFML\Graphics.hpp>

namespace net
{
	class Entity
	{
	public:
		sf::RectangleShape collisionRect; // for collision
		sf::Sprite sprite;
		sf::Text text;

		int spriteWidth = 0;
		int spriteHeight = 0;

		bool isAlive = true;
		//bool isDeleted = false;

		int x = 0;
		int y = 0;
	};

	class Player : public Entity
	{
	public:
		int id = 1;
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
				std::cout << collisionRect.getPosition().y << std::endl;
				std::cout << collisionRect_y << std::endl;
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
				std::cout << collisionRect.getPosition().y << std::endl;
				std::cout << collisionRect_y << std::endl;
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

	sf::Packet& operator <<(sf::Packet& packet, const Player& m)
	{
		return packet << m.id << m.velocity << m.attackDamage << m.direction << m.x << m.y << m.hp << m.powerUpLevel << m.canMoveUp << m.canMoveDown << m.canMoveLeft << m.canMoveRight << m.isAlive << m.collisionRect_x << m.collisionRect_y << m.projectile_x << m.projectile_y << m.shooted << m.projectileAlive;
	}

	sf::Packet& operator >>(sf::Packet& packet, Player& m)
	{

		return packet >> m.id >> m.velocity >> m.attackDamage >> m.direction >> m.x >> m.y >> m.hp >> m.powerUpLevel >> m.canMoveUp >> m.canMoveDown >> m.canMoveLeft >> m.canMoveRight >> m.isAlive >> m.collisionRect_x >> m.collisionRect_y >> m.projectile_x >> m.projectile_y >> m.shooted >> m.projectileAlive;
	}
}