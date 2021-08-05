#include "item.h"

Item::Item(int temp_x, int temp_y, int width, int height, int temp_type)
{
	spriteWidth = width;
	spriteHeight = height;
    x = temp_x;
    y = temp_y;
    type = temp_type;
    if (type == COIN)
    {
        scaleValue = 0.2;
    }
    else if (type == POWERUP)
    {
        scaleValue = 0.4;

    }

	collisionRect.setSize(sf::Vector2f(spriteWidth, spriteHeight));
	collisionRect.setPosition(0, 0);
	collisionRect.setFillColor(sf::Color::Cyan);
    collisionRect.setScale(sf::Vector2f(scaleValue, scaleValue));
    sprite.setScale(sf::Vector2f(scaleValue, scaleValue));
	sprite.setTextureRect(sf::IntRect(x, y, spriteWidth, spriteHeight));
}

Item::~Item()
{

}

void Item::update()
{
    animate();
	sprite.setPosition(collisionRect.getPosition());
    text.setPosition(collisionRect.getPosition().x, collisionRect.getPosition().y - 50);
}

void Item::animate()
{
    sprite.setTextureRect(sf::IntRect(animateSpriteNumber * spriteWidth, y , spriteWidth, spriteHeight));

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