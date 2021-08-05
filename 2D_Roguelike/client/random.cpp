#include "random.h"

int generateRandom(int max)
{
	int randomNumber = rand();
	int random = (randomNumber % max) + 1;
	
	return random;
}

int generateRandomFromZero(int max)
{
	int randomNumber = rand();
	int random = (randomNumber % max);

	return random;
}

bool generateRandomBool()
{
	int randomNumber = rand();
	int random = (randomNumber % 2) + 1;

	if (random == 1)
		return true;
	else
		return false;
}