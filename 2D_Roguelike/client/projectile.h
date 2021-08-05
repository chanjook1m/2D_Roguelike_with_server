#pragma once

#include "entity.h"

class Projectile: public Entity
{
public:
	float velocity = 0.f;
	float attackDamage = 0.f;
	int direction = 0; // 1 = up, 2 = down, 3 = left, 4 = right
	int lifetime = 100;
	int lifetimeCounter = 0;
	int id = 0;

	Projectile();
	~Projectile();

	void update();

};

