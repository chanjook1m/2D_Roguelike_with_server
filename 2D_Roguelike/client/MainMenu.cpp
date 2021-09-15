#include "MainMenu.hpp"

#include "GamePlay.hpp"

#include "Chatbox.h"
#include "Inputbox.h"

#include <SFML/Window/Event.hpp>
#define RESOURCE_DIR (std::string)"C:\\Users\\1z3r0\\Desktop\\game\\2D_Roguelike\\Resources\\"
sf::Font font;

ChatBox chatBox(sf::Vector2f(50, 100), 300, 5, 20, 15, font);
InputBox inputBox1(sf::Vector2f(500, 500), 120, 50, 20, 15, font);
InputBox inputBox2(sf::Vector2f(500, 600), 120, 50, 20, 15, font);

MainMenu::MainMenu(std::shared_ptr<Context>& context)
	: m_context(context), m_isPlayButtonSelected(true),
	m_isPlayButtonPressed(false), m_isExitButtonSelected(false),
	m_isExitButtonPressed(false)
{

}

MainMenu::~MainMenu()
{

}

void MainMenu::Init()
{

	// add font
	
    if (!font.loadFromFile(RESOURCE_DIR + "godoMaum.ttf"))
    {
        std::cout << "Error" << std::endl;
    }
	
    // Title
    m_gameTitle.setFont(font);
    m_gameTitle.setString("Roguelike Game");
 
    m_gameTitle.setOrigin(m_gameTitle.getLocalBounds().width / 2,
        m_gameTitle.getLocalBounds().height / 2);
    m_gameTitle.setPosition(m_context->m_window->getSize().x / 2,
        m_context->m_window->getSize().y / 2 - 150.f);

    // Play Button
    m_playButton.setFont(font);
    m_playButton.setString("Play");
    m_playButton.setOrigin(m_playButton.getLocalBounds().width / 2,
        m_playButton.getLocalBounds().height / 2);
    m_playButton.setPosition(m_context->m_window->getSize().x / 2,
        m_context->m_window->getSize().y / 2 - 25.f);
    m_playButton.setCharacterSize(20);

    // Exit Button
    m_exitButton.setFont(font);
    m_exitButton.setString("Exit");
    m_exitButton.setOrigin(m_exitButton.getLocalBounds().width / 2,
        m_exitButton.getLocalBounds().height / 2);
    m_exitButton.setPosition(m_context->m_window->getSize().x / 2,
        m_context->m_window->getSize().y / 2 + 25.f);
    m_exitButton.setCharacterSize(20);
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

        /*chatBox.handleEvent(event);
        std::string toBePushed;*/
        inputBox1.handleEvent(event, *m_context->m_window);
        inputBox2.handleEvent(event, *m_context->m_window);
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

    if (m_isPlayButtonPressed)
    {
        m_context->m_states->Add(std::make_unique<GamePlay>(m_context), true);
    }
    else if (m_isExitButtonPressed)
    {
        m_context->m_window->close();
    }

    /*if (isSelected)
        text.setPosition(520, 520);
    else if (isSelected2)
        text.setPosition(520, 720);*/
}

void MainMenu::Draw()
{
    
    chatBox.setFillColor(sf::Color::Cyan);
    chatBox.setOutlineColor(sf::Color::Black);
    chatBox.setCharColor(sf::Color::Black);

    m_context->m_window->clear(sf::Color::Blue);
    chatBox.update();
    chatBox.draw(*(m_context->m_window));

    inputBox1.update();
    inputBox1.draw(*(m_context->m_window));

    inputBox2.update();
    inputBox2.draw(*(m_context->m_window));

    m_context->m_window->draw(m_gameTitle);
    m_context->m_window->draw(m_playButton);
    m_context->m_window->draw(m_exitButton);

    m_context->m_window->display();
}