#include "player.h"


Player::Player(int width, int height)
{
    spriteWidth = width;
    spriteHeight = height;
	collisionRect.setSize(sf::Vector2f(spriteWidth, spriteHeight));	
	collisionRect.setPosition(300, 400);
	collisionRect.setFillColor(sf::Color::Blue);
    velocity = 2;
    sprite.setTextureRect(sf::IntRect(0,0, spriteWidth, spriteHeight));
	sprite.setPosition(300, 400);
}

Player::~Player()
{

}

void Player::update()
{
    move();
	sprite.setPosition(collisionRect.getPosition());
    text.setPosition(collisionRect.getPosition().x - 5, collisionRect.getPosition().y - 30);
}

void Player::move()
{
    if (isMainPlayer)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && canMoveUp)
        {
            //collisionRect.move(0.f, -velocity);
            sprite.setTextureRect(sf::IntRect(walkSpriteNumber * spriteWidth, spriteHeight * 1, spriteWidth, spriteHeight));
            direction = 1;
            updated = true;

        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && canMoveDown)
        {
            //collisionRect.move(0.f, velocity);
            sprite.setTextureRect(sf::IntRect(walkSpriteNumber * spriteWidth, 0, spriteWidth, spriteHeight));
            direction = 2;
            updated = true;

        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && canMoveLeft)
        {
            //collisionRect.move(-velocity, 0.f);
            sprite.setTextureRect(sf::IntRect(walkSpriteNumber * spriteWidth, spriteHeight * 2, spriteWidth, spriteHeight));
            direction = 3;
            updated = true;

        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && canMoveRight)
        {
            //collisionRect.move(velocity, 0.f);
            sprite.setTextureRect(sf::IntRect(walkSpriteNumber * spriteWidth, spriteHeight * 3, spriteWidth, spriteHeight));
            direction = 4;
            updated = true;

        }
        else
        {
            canMoveUp = true;
            canMoveDown = true;
            canMoveLeft = true;
            canMoveRight = true;
            updated = false;
            direction = 0;
        }   
    }
    else
    {
        if (direction == 1)
        {
            //collisionRect.move(0.f, -velocity);
            sprite.setTextureRect(sf::IntRect(walkSpriteNumber * spriteWidth, spriteHeight * 1, spriteWidth, spriteHeight));
            direction = 0;
            updated = true;

        }
        else if (direction == 2)
        {
            //collisionRect.move(0.f, velocity);
            sprite.setTextureRect(sf::IntRect(walkSpriteNumber * spriteWidth, 0, spriteWidth, spriteHeight));
            direction = 0;
            updated = true;

        }
        else if (direction == 3)
        {
            //collisionRect.move(-velocity, 0.f);
            sprite.setTextureRect(sf::IntRect(walkSpriteNumber * spriteWidth, spriteHeight * 2, spriteWidth, spriteHeight));
            direction = 0;
            updated = true;

        }
        else if (direction == 4)
        {
            //collisionRect.move(velocity, 0.f);
            sprite.setTextureRect(sf::IntRect(walkSpriteNumber * spriteWidth, spriteHeight * 3, spriteWidth, spriteHeight));
            direction = 0;
            updated = true;

        }
        else
        {
            canMoveUp = true;
            canMoveDown = true;
            canMoveLeft = true;
            canMoveRight = true;
            updated = false;
            
        }
    }
    
    walkSpriteNumber++;
    if (walkSpriteNumber == 7) {
        walkSpriteNumber = 0;
    }
}