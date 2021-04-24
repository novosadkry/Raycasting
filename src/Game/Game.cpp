#include "Game.hpp"

Game::Game(std::unique_ptr<sf::RenderWindow> window)
    : m_Window(std::move(window)) { }

void Game::PollEvents()
{
    sf::Event event;
    while (m_Window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            Exit();
    }
}

void Game::Update()
{

}

void Game::Render()
{

}

bool Game::IsRunning()
{
    return m_Window->isOpen();
}

void Game::Exit()
{
    m_Window->close();
}