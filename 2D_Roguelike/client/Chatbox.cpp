#include "ChatBox.h"
#include <iostream>

ChatBox::ChatBox(const sf::Vector2f pos, const float length_, const float thickness_,
	const int charSize_, const std::size_t historyLength_, const sf::Font& font) :
	thickness(thickness_), charSize(charSize_), historyLength(historyLength_), length(length_),
	box1(sf::Vector2f(length, thickness* (historyLength)+charSize * historyLength)),
	box2(sf::Vector2f(length - length/5 - 20, thickness * 2 + charSize)),
	box3(sf::Vector2f(length/5, thickness * 2 + charSize))
{

	box1.setPosition(pos);
	float x2 = box1.getGlobalBounds().left;
	float y2 = box1.getGlobalBounds().top + box1.getGlobalBounds().height + 3 * thickness;
	box2.setPosition(x2, y2);

	box3.setPosition(x2, y2);

	box1.setOutlineThickness(thickness);
	box2.setOutlineThickness(thickness);
	
	box3.setOutlineThickness(thickness);

	buttonText.setFont(font);
	buttonText.setString("Send");
	buttonText.setCharacterSize(charSize);

	text.setFont(font);
	text.setCharacterSize(charSize);
	text.setPosition(x2 + thickness, y2 + thickness);

	historyText.setFont(font);
	historyText.setCharacterSize(charSize);


}

ChatBox::~ChatBox(void)
{
}

void ChatBox::update()
{
	//text.setString(buffer);
	text.setString(s);
}

void ChatBox::handleEvent(sf::Event& event, sf::RenderWindow& window)
{
	if (event.type == sf::Event::TextEntered)
	{
		char code = static_cast<char>(event.text.unicode);

		if (event.text.unicode == 13)//enter
		{
			//onEnter(buffer);
			s.clear();
		}

		else if (code != '\b')
			s += event.text.unicode;//buffer.push_back(code);
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
		if (event.key.code == sf::Mouse::Left)
		{
			
			if (sf::Mouse::getPosition(window).x >= box3.getPosition().x
				&& sf::Mouse::getPosition(window).x <= box3.getPosition().x + box1.getSize().x - 60
				&& sf::Mouse::getPosition(window).y >= box3.getPosition().y
				&& sf::Mouse::getPosition(window).y <= box3.getPosition().y + box3.getSize().y)
				// The box has been selected
				// Toggle the boolean
			{
				std::cout << "[aaaaaaaaaaaaaaa] send clicked" << std::endl;
				s.clear();
				//isSelected = !isSelected;

				//std::string newStr1 = inputBox1.s.toAnsiString();
				//std::string newStr2 = inputBox2.s.toAnsiString();
				//std::string newStr = newStr1 + ";" + sha256(newStr2);

				//std::cout << newStr << std::endl; 

				//client.WriteOperation(0, "127.0.0.1", 5556, net::handler, player_id, newStr, 2);
				//std::this_thread::sleep_for(std::chrono::seconds(1));
				//std::cout << net::id << std::endl;

				//if (net::connected)
				//{
				//    client.WriteOperation(5, "127.0.0.1", 5555, net::handler, player_id, "", 0);
				//    std::this_thread::sleep_for(std::chrono::seconds(3));
				//}



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

void ChatBox::push(const std::string& s)
{
	if (s.size() > 0)
	{
		history.push_front(s);
		if (history.size() > historyLength)
			history.pop_back();
	}
}

void ChatBox::connectOnEnter(std::function<void(const std::string& s)> func)
{
	onEnter = func;
}

void ChatBox::draw(sf::RenderWindow& window)
{
	window.draw(box1);
	window.draw(box2);
	window.draw(box3);
	window.draw(text);
	window.draw(buttonText);

	float x = box1.getGlobalBounds().left + 2 * thickness;
	float y = box1.getGlobalBounds().top + box1.getGlobalBounds().height - thickness - charSize;

	for (std::size_t i = 0; i < history.size(); ++i)
	{
		historyText.setString(history[i]);
		historyText.setPosition(x, y - i * charSize - i * thickness - thickness);
		window.draw(historyText);
	}
}


void ChatBox::setOutlineColor(const sf::Color& color)
{
	box1.setOutlineColor(color);
	box2.setOutlineColor(color);
	box3.setOutlineColor(color);
}

void ChatBox::setFillColor(const sf::Color& color)
{
	box1.setFillColor(color);
	box2.setFillColor(color);
	box3.setFillColor(color);
}

void ChatBox::setCharColor(const sf::Color& color)
{
	buttonText.setFillColor(color);
	text.setFillColor(color);
	historyText.setFillColor(color);
}
