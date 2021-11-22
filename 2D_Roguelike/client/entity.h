#pragma once

#include <SFML\Audio.hpp>
#include <SFML\Graphics.hpp>
#include <iostream>

using namespace std;

class Entity
{
public:
	sf::RectangleShape collisionRect; // for collision
	sf::Sprite sprite;
	sf::Text text;

	int spriteWidth = 0;
	int spriteHeight = 0;	

	bool isCollide = false;
	bool isAlive = true;
	//bool isDeleted = false;
	int id;
	int x = 0;
	int y = 0;
};

