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
    auto& window = *m_Window;
    auto& level = *m_CurrentLevel;

    window.clear(sf::Color::Black);

    // sf::RectangleShape rect(sf::Vector2f(10.0f, 10.0f));

    // for (int x = 0; x < level.GetGrid().SizeX(); x++)
    // {
    //     for (int y = 0; y < level.GetGrid().SizeY(); y++)
    //     {
    //         rect.setPosition(x * 11.0f, y * 11.0f);
    //         window.draw(rect);
    //     }
    // }

    window.display();
}