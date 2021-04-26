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

static void RenderMinimap(sf::RenderWindow& window, Level& level, sf::Vector2i size, float spacing = 1.0f, sf::Vector2f origin = {0, 0})
{
    float cellWidth = (float) size.x / level.GetGrid().SizeX();
    float cellHeight = (float) size.y / level.GetGrid().SizeY();

    sf::RectangleShape rect(sf::Vector2f(cellWidth, cellHeight));

    for (int x = 0; x < level.GetGrid().SizeX(); x++)
    {
        for (int y = 0; y < level.GetGrid().SizeY(); y++)
        {
            rect.setPosition(
                x * (cellWidth + spacing) + origin.x,
                y * (cellHeight + spacing) + origin.y
            );

            window.draw(rect);
        }
    }
}

void Game::Render()
{
    auto& window = *m_Window;
    auto& level = *m_CurrentLevel;

    window.clear(sf::Color::Black);

    RenderMinimap(window, level, {100, 100});

    window.display();
}