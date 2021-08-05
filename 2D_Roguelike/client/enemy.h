#pragma once

#include "entity.h"
#include "random.h"

class Enemy: public Entity
{
public:
	float velocity = 1.0f;
	float attackDamage = 5.0f;
	int walkSpriteNumber = 0;
	int direction = 0;
	int delayCounter = 0;
	int movementDelay = 50;
	int hp = 3;
	int maxHp = 3;

	bool canMoveUp = true;
	bool canMoveDown = true;
	bool canMoveLeft = true;
	bool canMoveRight = true;

	int aggroedBy = 0;

	Enemy(int x, int y, int width, int height);
	~Enemy();

	void update();
	void move();

};

