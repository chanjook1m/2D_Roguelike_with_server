#pragma once

#include "entity.h"

class Wall: public Entity
{
public:
	Wall(int x, int y, int width, int height);
	~Wall();

	bool destructible = false;
	int hp = 3;

	void update();
};

