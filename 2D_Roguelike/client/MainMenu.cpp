#include "MainMenu.hpp"

#include "GamePlay.hpp"

#include "Inputbox.h"
#include "client_interface.hpp"

#include "sha256.h"

#include <SFML/Window/Event.hpp>
#define RESOURCE_DIR (std::string)"C:\\Users\\1z3r0\\Desktop\\game\\2D_Roguelike\\Resources\\"

static net::AsyncTCPClient client;
int player_id = 0;

sf::Font font;

//static ChatBox chatBox(sf::Vector2f(50, 100), 300, 5, 20, 15, font);
InputBox* inputBox1;// (sf::Vector2f(500, 500), 120, 50, 40, 15, font, 0);
InputBox* inputBox2;// (sf::Vector2f(500, 600), 120, 50, 40, 15, font, 1);

sf::RectangleShape button(sf::Vector2f(120, 50));
bool isSelected = false;
sf::String s;
sf::Text text;
sf::Text loginResultMessage;

MainMenu::MainMenu(std::shared_ptr<Context>& context)
	: m_context(context), m_isPlayButtonSelected(true),
	m_isPlayButtonPressed(false), m_isExitButtonSelected(false),
	m_isExitButtonPressed(false)
{
    inputBox1 = new InputBox(sf::Vector2f(m_context->m_window->getSize().x / 2 - 50, m_context->m_window->getSize().y / 2 - 25.f),
        120, 50, 40, 15, font, 0);
    inputBox2 = new InputBox(sf::Vector2f(m_context->m_window->getSize().x / 2 - 50, m_context->m_window->getSize().y / 2 + 75.f),
        120, 50, 40, 15, font, 1);
}

MainMenu::~MainMenu()
{

}

void MainMenu::Init()
{
    button.setPosition(m_context->m_window->getSize().x / 2 - 50, m_context->m_window->getSize().y / 2 + 175.f);

	// add font
	
    if (!font.loadFromFile(RESOURCE_DIR + "godoMaum.ttf"))
    {
        std::cout << "Error" << std::endl;
    }

    text.setFont(font);
    text.setString(L"로그인");
    text.setPosition(button.getPosition().x + 40, button.getPosition().y);
    text.setFillColor(sf::Color::Black);

    loginResultMessage.setFont(font);
    loginResultMessage.setFillColor(sf::Color::Black);
    loginResultMessage.setPosition(button.getPosition().x, button.getPosition().y + 50);
    
	
    // Title
    m_gameTitle.setFont(font);
    m_gameTitle.setString("Roguelike Game");
 
    m_gameTitle.setOrigin(m_gameTitle.getLocalBounds().width / 2,
        m_gameTitle.getLocalBounds().height / 2);
    m_gameTitle.setPosition(m_context->m_window->getSize().x / 2,
        m_context->m_window->getSize().y / 2 - 150.f);

    //// Play Button
    //m_playButton.setFont(font);
    //m_playButton.setString("Play");
    //m_playButton.setOrigin(m_playButton.getLocalBounds().width / 2,
    //    m_playButton.getLocalBounds().height / 2);
    //m_playButton.setPosition(m_context->m_window->getSize().x / 2,
    //    m_context->m_window->getSize().y / 2 - 25.f);
    //m_playButton.setCharacterSize(20);

    //// Exit Button
    //m_exitButton.setFont(font);
    //m_exitButton.setString("Exit");
    //m_exitButton.setOrigin(m_exitButton.getLocalBounds().width / 2,
    //    m_exitButton.getLocalBounds().height / 2);
    //m_exitButton.setPosition(m_context->m_window->getSize().x / 2,
    //    m_context->m_window->getSize().y / 2 + 25.f);
    //m_exitButton.setCharacterSize(20);
}

void MainMenu::ProcessInput()
{
    sf::Event event;
    while (m_context->m_window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            m_context->m_window->close();
        }
        else if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::Up:
            {
                if (!m_isPlayButtonSelected)
                {
                    m_isPlayButtonSelected = true;
                    m_isExitButtonSelected = false;
                }
                break;
            }
            case sf::Keyboard::Down:
            {
                if (!m_isExitButtonSelected)
                {
                    m_isPlayButtonSelected = false;
                    m_isExitButtonSelected = true;
                }
                break;
            }
            case sf::Keyboard::Return:
            {
                m_isPlayButtonPressed = false;
                m_isExitButtonPressed = false;

                if (m_isPlayButtonSelected)
                {
                    m_isPlayButtonPressed = true;
                }
                else
                {
                    m_isExitButtonPressed = true;
                }

                break;
            }
            default:
            {
                

                break;
            }
            }
        }
        else if (event.type == sf::Event::MouseButtonReleased)
        {
            if (event.key.code == sf::Mouse::Left)
            {
                if (sf::Mouse::getPosition(*(m_context->m_window)).x >= button.getPosition().x
                    && sf::Mouse::getPosition(*(m_context->m_window)).x <= button.getPosition().x + button.getSize().x
                    && sf::Mouse::getPosition(*(m_context->m_window)).y >= button.getPosition().y
                    && sf::Mouse::getPosition(*(m_context->m_window)).y <= button.getPosition().y + button.getSize().y)
                    // The box has been selected
                    // Toggle the boolean
                {
                    isSelected = !isSelected;

                    std::string newStr1 = inputBox1->s.toAnsiString();
                    std::string newStr2 = inputBox2->s.toAnsiString();
                    std::string newStr = newStr1 + ";" + sha256(newStr2);

                    std::cout << newStr << std::endl; 

                    client.WriteOperation(0, "127.0.0.1", 5556, net::handler, player_id, newStr, 2);

                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    std::cout << net::id << std::endl;

                    if (net::connected)
                    {
                        client.WriteOperation(5, "127.0.0.1", 5555, net::handler, player_id, "", 0);
                        
                        std::this_thread::sleep_for(std::chrono::seconds(3));
                    }

                    
                    
                    loginResultMessage.setString(net::connected ? L"로그인 성공" : L"로그인 실패");
                }
            }
        }
        //else if (event.type == sf::Event::TextEntered)
        //{
        //    char code = static_cast<char>(event.text.unicode);

        //    if (code != '\b')
        //        s += event.text.unicode;//buffer.push_back(code);
        //    else if (code == '\b')
        //    {
        //        /*if (buffer.size() > 0)
        //            buffer.pop_back();*/
        //        if (s.getSize() > 0) {
        //            //std::cout << "문자열길이 : " << s.getSize() << std::endl;
        //            s.erase(s.getSize() - 1); //마지막글자를 지운다.
        //        }
        //    }
        //}

        //std::cout << isSelected << std::endl;
        /*chatBox.handleEvent(event);
        std::string toBePushed;*/
        inputBox1->handleEvent(event, *m_context->m_window);
        inputBox2->handleEvent(event, *m_context->m_window);
    }
}

void MainMenu::Update(sf::Time deltaTime)
{
    if (m_isPlayButtonSelected)
    {
        m_playButton.setFillColor(sf::Color::Black);
        m_exitButton.setFillColor(sf::Color::White);
    }
    else
    {
        m_exitButton.setFillColor(sf::Color::Black);
        m_playButton.setFillColor(sf::Color::White);
    }
    
    if (net::connected)
    {
        //std::cout << "connected" << std::endl;
        m_context->m_states->Add(std::make_unique<GamePlay>(m_context), true);
    }
    else
    {
        //std::cout << "not connected" << std::endl;
    }
    /*else if (m_isExitButtonPressed)
    {
        m_context->m_window->close();
    }*/
}

void MainMenu::Draw()
{
    
    /*chatBox.setFillColor(sf::Color::Cyan);
    chatBox.setOutlineColor(sf::Color::Black);
    chatBox.setCharColor(sf::Color::Black);*/

    m_context->m_window->clear(sf::Color::Blue);
    //chatBox.update();
    //chatBox.draw(*(m_context->m_window));

    inputBox1->update();
    inputBox1->draw(*(m_context->m_window));

    inputBox2->update();
    inputBox2->draw(*(m_context->m_window));

   

    m_context->m_window->draw(m_gameTitle);
    m_context->m_window->draw(m_playButton);
    m_context->m_window->draw(m_exitButton);

    m_context->m_window->draw(button);
    m_context->m_window->draw(text);
    m_context->m_window->draw(loginResultMessage);
    
    
    m_context->m_window->display();
}