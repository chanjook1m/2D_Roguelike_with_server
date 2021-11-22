#pragma once

#include <deque>
#include <string>
#include "SFML/Graphics.hpp"
#include <functional>

namespace sf
{
	class Packet;
}

class InputBox
{
private:
	const std::size_t historyLength;
	const int charSize;

	sf::RectangleShape box;  //histories go here

	bool isSelected = false;
	int type = 0;

	sf::Text text;
	sf::Text historyText;
	std::deque<std::string> history;

	std::string buffer;

	

	std::function<void(const std::string& s)> onEnter;
public:
	InputBox(const sf::Vector2f pos, const float width, const float height, const int charSize, const std::size_t historyLength, const sf::Font& font, int type);
	~InputBox();

	sf::String s;
		
	void setOutlineColor(const sf::Color& color);
	void setFillColor(const sf::Color& color);
	void setCharColor(const sf::Color& color);



	void update();
	void handleEvent(sf::Event& event, sf::RenderWindow& window);

	void push(const std::string& s);
	void connectOnEnter(std::function<void(const std::string& s)> func);

	void draw(sf::RenderWindow& window);

};
