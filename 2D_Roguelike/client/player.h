#pragma once

#include "entity.h"

class Player: public Entity
{
public:
	int id = 1;
	int velocity = 2;	
	float attackDamage = 1.0f;
	int powerUpLevel = 1;
	int maxPowerUpLevel = 4;
	int walkSpriteNumber = 0;
	int direction = 0; // 1 = up, 2 = down, 3 = left, 4 = right
	int hp = 100;
	int maxHp = 100;
	int score = 0;

	bool canMoveUp = true;
	bool canMoveDown = true;
	bool canMoveLeft = true;
	bool canMoveRight = true;

	int collisionRect_x = 300;
	int collisionRect_y = 400;
	
	bool updated = false;
	bool isMainPlayer = false;
	bool shooted = false;
	int projectile_x = collisionRect_x;
	int projectile_y = collisionRect_y;
	bool projectileAlive = false;
	Player(int width, int height);
	~Player();

	void update();
	void move();
};

