#pragma once

#include "entity.h"

class Item: public Entity
{
public:
	Item(int x, int y, int width, int height, int type);
	~Item();

	enum types
	{
		COIN = 1,
		POWERUP,
	};

	int animateSpriteNumber = 0;
	int delayCounter = 0;
	int animateDelay = 5;
	int type = 0;
	float scaleValue = 0.f;
	bool inShop = false;
	int cost = 0;

	void update();
	void animate();
};

