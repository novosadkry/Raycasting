#include "MiniMap.hpp"

#include <Game/Game.hpp>
#include <Game/Level.hpp>
#include <Game/Ray.hpp>
#include <Game/Render/LevelView.hpp>
#include <Utils/Math.hpp>

#include "Player.hpp"

static void RenderMinimap(sf::RenderWindow& window, Level& level, Player& player, sf::Vector2i size, float spacing = 1.0f, sf::Vector2f origin = {0, 0})
{
    // Initialize cell shape
    float cellWidth = (float) size.x / level.GetGrid().GetSize().x;
    float cellHeight = (float) size.y / level.GetGrid().GetSize().y;
    sf::RectangleShape cShape(sf::Vector2f(cellWidth, cellHeight));

    // Render individual cells
    for (int x = 0; x < level.GetGrid().GetSize().x; x++)
    {
        for (int y = 0; y < level.GetGrid().GetSize().y; y++)
        {
            Cell cell = level.GetGrid().Get(x, y);

            switch (cell)
            {
                case Wall:
                    cShape.setFillColor(sf::Color::Black);
                    cShape.setOutlineColor(sf::Color::White);
                    break;

                case Empty:
                    cShape.setFillColor(sf::Color::White);
                    cShape.setOutlineColor(sf::Color::Black);
                    break;
            }

            cShape.setOutlineThickness(spacing);
            cShape.setPosition(
                x * cellWidth + origin.x,
                y * cellHeight + origin.y
            );

            window.draw(cShape);
        }
    }

    // Normalize player values
    float pShapeRadius = (player.GetRadius() / level.GetSize().x) * size.x;
    sf::Vector2f pShapePos = {
        (player.GetPosition().x / level.GetSize().x) * size.x,
        (player.GetPosition().y / level.GetSize().y) * size.y
    };

    Ray ray;
    Ray::Cast(level, player.GetPosition(), player.GetRotation(), ray);

    // Draw ray hit
    sf::CircleShape hitCircle(pShapeRadius / 2);
    hitCircle.setOrigin(pShapeRadius / 2, pShapeRadius / 2);
    hitCircle.setFillColor(sf::Color::Red);
    sf::Vector2f hitPos = {
        (ray.hitPos.x / level.GetSize().x) * size.x,
        (ray.hitPos.y / level.GetSize().y) * size.y
    };
    hitCircle.setPosition(pShapePos + hitPos + origin);

    // Initialize player shape
    sf::CircleShape pShape(pShapeRadius);
    pShape.setOrigin(pShapeRadius, pShapeRadius);
    pShape.setFillColor(sf::Color::Green);
    pShape.setOutlineColor(sf::Color::Black);
    pShape.setOutlineThickness(1);
    pShape.setPosition(pShapePos + origin);

    // Initialize player direction shape
    sf::RectangleShape pDirShape(sf::Vector2f(pShapeRadius * 2, pShapeRadius / 2));
    pDirShape.setFillColor(sf::Color::Blue);
    pDirShape.setPosition(pShape.getPosition() + origin);
    pDirShape.rotate(player.GetRotation() * Math::Rad2Deg);

    // Render player
    // - Sets shape position relative to minimap size and level size
    // - Shows player forward direction
    // - Renders ray hit
    window.draw(pShape);
    window.draw(pDirShape);
    window.draw(hitCircle);
}

void MiniMap::Render(float dt)
{
    RenderMinimap(Game::Get().GetWindow(), Game::Get().GetCurrentLevel(), *m_Player, m_Size);
}