#include "InputBox.h"
#include <iostream>

InputBox::InputBox(const sf::Vector2f pos, const float width_, const float height_, const int charSize_,
	const std::size_t historyLength_, const sf::Font& font, int type_) : 
	charSize(charSize_), historyLength(historyLength_), type(type_)
{
	box.setPosition(pos);
	box.setSize(sf::Vector2f(width_, height_));
	

	text.setFont(font);
	text.setCharacterSize(charSize);
	text.setFillColor(sf::Color::Black);
	text.setPosition(pos.x + 30, pos.y);

	historyText.setFont(font);
	historyText.setCharacterSize(charSize);
}

InputBox::~InputBox(void)
{
}

void InputBox::update()
{
	//text.setString(buffer);
	if (type == 0)
		text.setString(s);
	else if (type == 1)
	{
		sf::String new_s;
		for (int i = 0; i < s.getSize(); i++)
		{
			new_s += "*";
		}
		text.setString(new_s);
	}
}

void InputBox::handleEvent(sf::Event& event, sf::RenderWindow& window)
{
	if (event.type == sf::Event::TextEntered && isSelected)
	{
		char code = static_cast<char>(event.text.unicode);

		if (event.text.unicode == 13)//enter
		{
			onEnter(buffer);
			buffer.clear();
		}

		else if (code != '\b')
		{

			s += event.text.unicode;//buffer.push_back(code);
		}
		else if (code == '\b')
		{
			/*if (buffer.size() > 0)
				buffer.pop_back();*/
			if (s.getSize() > 0) {
				//std::cout << "문자열길이 : " << s.getSize() << std::endl;
				s.erase(s.getSize() - 1); //마지막글자를 지운다.
			}
		}
	}
	else if (event.type == sf::Event::MouseButtonReleased)
	{
		isSelected = false;
		if (event.key.code == sf::Mouse::Left)
		{
			if (sf::Mouse::getPosition(window).x >= box.getPosition().x
				&& sf::Mouse::getPosition(window).x <= box.getPosition().x + box.getSize().x
				&& sf::Mouse::getPosition(window).y >= box.getPosition().y
				&& sf::Mouse::getPosition(window).y <= box.getPosition().y + box.getSize().y)
				// The box has been selected
				// Toggle the boolean
			{
				isSelected = !isSelected;
			}
		}
	}
	//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::BackSpace))
	//{
	//	if (s.getSize() > 0) {
	//		//std::cout << "문자열길이 : " << s.getSize() << std::endl;
	//		s.erase(s.getSize() - 1); //마지막글자를 지운다.
	//	}
	//}
}

void InputBox::push(const std::string& s)
{
	if (s.size() > 0)
	{
		history.push_front(s);
		if (history.size() > historyLength)
			history.pop_back();
	}
}

void InputBox::connectOnEnter(std::function<void(const std::string& s)> func)
{
	onEnter = func;
}

void InputBox::draw(sf::RenderWindow& window)
{
	window.draw(box);
	window.draw(text);

	float x = box.getGlobalBounds().left;
	float y = box.getGlobalBounds().top + box.getGlobalBounds().height - charSize;

	for (std::size_t i = 0; i < history.size(); ++i)
	{
		historyText.setString(history[i]);
		historyText.setPosition(x, y - i * charSize);
		window.draw(historyText);
	}
}


void InputBox::setOutlineColor(const sf::Color& color)
{
	box.setOutlineColor(color);
}

void InputBox::setFillColor(const sf::Color& color)
{
	box.setFillColor(color);
}

void InputBox::setCharColor(const sf::Color& color)
{
	text.setFillColor(color);
	historyText.setFillColor(color);
}
