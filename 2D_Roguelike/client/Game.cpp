#include <SFML/Graphics/CircleShape.hpp>

#include "Game.hpp"
#include "MainMenu.hpp"
#include <boost/asio.hpp>
#include "net.hpp"


Game::Game() : m_context(std::make_shared<Context>())
{
    m_context->m_window->create(sf::VideoMode(1000, 800), "Roguelike Game", sf::Style::Close);
    m_context->m_states->Add(std::make_unique<MainMenu>(m_context));
}

Game::~Game()
{
}

void Game::Run()
{
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    sf::Clock clock;
    sf::Time timeSinceLastFrame = sf::Time::Zero;

    boost::asio::io_context tcp_io_context;
    
    std::unique_ptr<std::thread> th(new std::thread([&]()
        {
            net::receiver2 r(tcp_io_context,
                "127.0.0.1",
                5557);

            tcp_io_context.run();
        }));

    while (m_context->m_window->isOpen())
    {
        timeSinceLastFrame += clock.restart();

        while (timeSinceLastFrame > TIME_PER_FRAME)
        {
            timeSinceLastFrame -= TIME_PER_FRAME;

            m_context->m_states->ProcessStateChange();
            m_context->m_states->GetCurrent()->ProcessInput();
            m_context->m_states->GetCurrent()->Update(TIME_PER_FRAME);
            m_context->m_states->GetCurrent()->Draw();
        }
    }
}