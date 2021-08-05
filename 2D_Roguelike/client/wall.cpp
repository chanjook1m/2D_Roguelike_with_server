#include "wall.h"

Wall::Wall(int x, int y, int width, int height)
{
    spriteWidth = width;
    spriteHeight = height;
    collisionRect.setSize(sf::Vector2f(spriteWidth, spriteHeight));
    collisionRect.setPosition(0, 0);
    collisionRect.setFillColor(sf::Color::White);
    collisionRect.setScale(3.0, 3.0);
    sprite.setTextureRect(sf::IntRect(x, y, spriteWidth, spriteHeight));
}

Wall::~Wall()
{

}

void Wall::update()
{
    sprite.setPosition(collisionRect.getPosition());
}