#include "MiniMap.hpp"

#include <Game/Game.hpp>
#include <Game/Level.hpp>
#include "Player.hpp"

static void RenderMinimap(sf::RenderWindow& window, Level& level, Player& player, sf::Vector2i size, float spacing = 1.0f, sf::Vector2f origin = {0, 0})
{
    // Initialize cell shape
    float cellWidth = (float) size.x / level.GetGrid().SizeX();
    float cellHeight = (float) size.y / level.GetGrid().SizeY();
    sf::RectangleShape cell(sf::Vector2f(cellWidth, cellHeight));

    // Render individual cells
    for (int x = 0; x < level.GetGrid().SizeX(); x++)
    {
        for (int y = 0; y < level.GetGrid().SizeY(); y++)
        {
            cell.setPosition(
                x * (cellWidth + spacing) + origin.x,
                y * (cellHeight + spacing) + origin.y
            );

            window.draw(cell);
        }
    }

    // Normalize player values
    float pShapeRadius = (player.GetRadius() / level.GetSize().x) * size.x;
    sf::Vector2f pShapePos = {
        (player.GetPosition().x / level.GetSize().x) * size.x,
        (player.GetPosition().y / level.GetSize().y) * size.y
    };

    // Initialize player shape
    sf::CircleShape pShape(pShapeRadius);
    pShape.setOrigin(pShapeRadius, pShapeRadius);
    pShape.setFillColor(sf::Color::Green);
    pShape.setOutlineColor(sf::Color::Black);
    pShape.setOutlineThickness(1);
    pShape.setPosition(pShapePos);

    // Initialize player direction shape
    sf::RectangleShape pDirShape(sf::Vector2f(pShapeRadius * 2, pShapeRadius / 2));
    pDirShape.setFillColor(sf::Color::Blue);
    pDirShape.rotate(player.GetDirection());
    pDirShape.setPosition(pShape.getPosition());

    // Render player
    // - Sets shape position relative to minimap size and level size
    // - Shows player forward direction
    window.draw(pShape);
    window.draw(pDirShape);
}

void MiniMap::Render(float dt)
{
    RenderMinimap(Game::Get().GetWindow(), Game::Get().GetCurrentLevel(), *m_Player, {100, 100});
}