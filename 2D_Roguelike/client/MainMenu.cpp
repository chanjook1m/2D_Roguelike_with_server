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

InputBox* inputBox1;
InputBox* inputBox2;

sf::RectangleShape button(sf::Vector2f(120, 50));
sf::RectangleShape button2(sf::Vector2f(120, 50));
bool isSelected = false;
bool isClicked = false;
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
    button2.setPosition(m_context->m_window->getSize().x / 2 - 50, m_context->m_window->getSize().y / 2 + 245.f);

	// add font
	
    if (!font.loadFromFile(RESOURCE_DIR + "godoMaum.ttf"))
    {
        std::cout << "Error" << std::endl;
    }

    text.setFont(font);
    text.setString(L"�α���");
    text.setPosition(button.getPosition().x + 40, button.getPosition().y);
    text.setFillColor(sf::Color::Black);

    loginResultMessage.setFont(font);
    loginResultMessage.setFillColor(sf::Color::White);
    loginResultMessage.setPosition(button.getPosition().x, button.getPosition().y + 50);
    
	
    // Title
    m_gameTitle.setFont(font);
    m_gameTitle.setString("Roguelike Game");
 
    m_gameTitle.setOrigin(m_gameTitle.getLocalBounds().width / 2,
        m_gameTitle.getLocalBounds().height / 2);
    m_gameTitle.setPosition(m_context->m_window->getSize().x / 2,
        m_context->m_window->getSize().y / 2 - 150.f);

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
                       
                    m_context->m_window->clear(sf::Color::Black);
                    loginResultMessage.setString(L"�α��� ��...");
                    m_context->m_window->draw(loginResultMessage);
                    m_context->m_window->display();

                    client.WriteOperation(0, "127.0.0.1", 5556, net::handler, player_id, newStr, 2);
                    
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    std::cout << net::id << std::endl;

                    if (net::connected)
                    {
                        client.WriteOperation(5, "127.0.0.1", 5555, net::handler, player_id, newStr, 0);
                        std::this_thread::sleep_for(std::chrono::seconds(3));
                    }

                    loginResultMessage.setString(L"");
                    loginResultMessage.setString(net::connected ? L"�α��� ����" : L"�α��� ����");
                    
                        
                }
                else if (sf::Mouse::getPosition(*(m_context->m_window)).x >= button2.getPosition().x
                    && sf::Mouse::getPosition(*(m_context->m_window)).x <= button2.getPosition().x + button2.getSize().x
                    && sf::Mouse::getPosition(*(m_context->m_window)).y >= button2.getPosition().y
                    && sf::Mouse::getPosition(*(m_context->m_window)).y <= button2.getPosition().y + button2.getSize().y)
                    // The box has been selected
                    // Toggle the boolean
                {
                    isClicked = !isClicked;
                
                }
                if (isClicked == true)
                {
                    button.setFillColor(sf::Color::Transparent);
                }
                else
                {
                    button.setFillColor(sf::Color::White);
                }
            }
        }
        
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
        m_context->m_states->Add(std::make_unique<GamePlay>(m_context), true);
    }
    else
    {
        // Do something
    }
    
}

void MainMenu::Draw()
{
    m_context->m_window->clear(sf::Color::Black);

    m_context->m_window->draw(loginResultMessage);

    inputBox1->update();
    inputBox1->draw(*(m_context->m_window));

    inputBox2->update();
    inputBox2->draw(*(m_context->m_window));

    m_context->m_window->draw(m_gameTitle);
    m_context->m_window->draw(m_playButton);
    m_context->m_window->draw(m_exitButton);

    m_context->m_window->draw(button);
    m_context->m_window->draw(button2);
    m_context->m_window->draw(text);
    
    m_context->m_window->display();
}