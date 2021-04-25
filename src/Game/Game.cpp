#include "Game.hpp"
#include "../Input/Input.hpp"

Game::Game(std::unique_ptr<sf::RenderWindow> window)
    : m_Window(std::move(window))
{
    Input::Setup(*this);
}

void Game::PollEvents()
{
    sf::Event event;
    while (m_Window->pollEvent(event))
        Input::HandleEvent(event);
}

void Game::Update()
{

}

void Game::Render()
{

}