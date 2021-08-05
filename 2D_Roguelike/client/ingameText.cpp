#include "ingameText.h"

IngameText::IngameText()
{
	text.setFillColor(sf::Color::Red);
	text.setCharacterSize(30);
	text.setPosition(0, 0);
	text.setString(testString);
}

IngameText::~IngameText()
{

}

void IngameText::update()
{
	text.move(0, -velocity);

	lifetimeCounter++;
	if (lifetimeCounter >= lifeTime)
	{
		isAlive = false;
	}
}