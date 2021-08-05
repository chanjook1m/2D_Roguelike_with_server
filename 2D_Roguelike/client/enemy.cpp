#include "enemy.h"

Enemy::Enemy(int tempX, int tempY, int width, int height)
{
    spriteWidth = width;
    spriteHeight = height;
    x = tempX;
    y = tempY;
    collisionRect.setSize(sf::Vector2f(spriteWidth, spriteHeight));
    collisionRect.setPosition(500, 200);
    collisionRect.setFillColor(sf::Color::Blue);    
    sprite.setTextureRect(sf::IntRect(x, y, spriteWidth, spriteHeight));
}

Enemy::~Enemy()
{

}

void Enemy::update()
{
    move();
    sprite.setPosition(collisionRect.getPosition());
    text.setPosition(collisionRect.getPosition().x - 5, collisionRect.getPosition().y - 30);
}

void Enemy::move()
{
    if (direction == 1 && canMoveUp)
    {
        collisionRect.move(0.f, -velocity);
        sprite.setTextureRect(sf::IntRect(walkSpriteNumber * spriteWidth + x, spriteHeight * 3 + y, spriteWidth, spriteHeight));
        canMoveUp = true;
        canMoveDown = true;
        canMoveLeft = true;
        canMoveRight = true;
    }
    else if (direction == 2 && canMoveDown)
    {
        collisionRect.move(0.f, velocity);
        sprite.setTextureRect(sf::IntRect(walkSpriteNumber * spriteWidth + x, 0 + y, spriteWidth, spriteHeight));
        canMoveUp = true;
        canMoveDown = true;
        canMoveLeft = true;
        canMoveRight = true;
    }
    else if (direction == 3 && canMoveLeft)
    {
        collisionRect.move(-velocity, 0.f);
        sprite.setTextureRect(sf::IntRect(walkSpriteNumber * spriteWidth + x, spriteHeight * 1 + y, spriteWidth, spriteHeight));
        canMoveUp = true;
        canMoveDown = true;
        canMoveLeft = true;
        canMoveRight = true;
    }
    else if (direction == 4 && canMoveRight)
    {
        collisionRect.move(velocity, 0.f);
        sprite.setTextureRect(sf::IntRect(walkSpriteNumber * spriteWidth + x, spriteHeight * 2 + y, spriteWidth, spriteHeight));
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
        delayCounter = 0;
        direction = generateRandom(4);
    }
}
