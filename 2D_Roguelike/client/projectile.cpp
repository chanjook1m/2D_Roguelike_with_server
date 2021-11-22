#include "projectile.h"

Projectile::Projectile()
{
	collisionRect.setSize(sf::Vector2f(8,8));
	collisionRect.setPosition(0, 0);
	collisionRect.setFillColor(sf::Color::Green);
	//sprite.setTextureRect(sf::IntRect(0,0,8,8));

	velocity = 3.0f;
	attackDamage = 1.0f;
}

Projectile::~Projectile()
{

}

void Projectile::update()
{
	if (direction == 1)
	{
		collisionRect.move(0, -velocity);
	}

	if (direction == 2 || direction == 0)
	{
		collisionRect.move(0, velocity);
	}

	if (direction == 3)
	{
		collisionRect.move(-velocity, 0);
	}

	if (direction == 4)
	{
		collisionRect.move(velocity, 0);
	}

	/*lifetimeCounter++;

	if (lifetimeCounter >= lifetime)
	{
		lifetimeCounter = 0;
		isAlive = false;
	}*/
	
	sprite.setPosition(collisionRect.getPosition());
}