#pragma once

#include "entity.h"

class IngameText: public Entity
{
public:
	IngameText();
	~IngameText();

	string testString = "start";
	float velocity = 2.0f;
	int lifetimeCounter = 0;
	int lifeTime = 50;

	void update();
};

