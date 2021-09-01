#include "MiniMap.hpp"

#include <Game/Game.hpp>
#include <Game/Level.hpp>
#include <Game/Ray.hpp>
#include <Game/Render/LevelView.hpp>
#include <Utils/Math.hpp>

#include "Player.hpp"

void MiniMap::RenderMiniMap()
{
    auto& window = Game::Get().GetWindow();
    auto& level  = Game::Get().GetCurrentLevel();

    auto player = m_Player.lock();

    // Initialize cell shape
    float cellWidth = (float) m_Size.x / level.GetGrid().GetSize().x;
    float cellHeight = (float) m_Size.y / level.GetGrid().GetSize().y;
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

            cShape.setOutlineThickness(m_Spacing);
            cShape.setPosition(
                x * cellWidth + m_Origin.x,
                y * cellHeight + m_Origin.y
            );

            window.draw(cShape);
        }
    }

    // Normalize player values
    float pShapeRadius = (player->GetRadius() / level.GetSize().x) * m_Size.x;
    sf::Vector2f pShapePos = {
        (player->GetPosition().x / level.GetSize().x) * m_Size.x + m_Origin.x,
        (player->GetPosition().y / level.GetSize().y) * m_Size.y + m_Origin.y
    };

    if (m_Flags & SHOW_PLAYER)
    {
        // Draw player shape
        sf::CircleShape pShape(pShapeRadius);
        pShape.setOrigin(pShapeRadius, pShapeRadius);
        pShape.setFillColor(sf::Color::Green);
        pShape.setOutlineColor(sf::Color::Black);
        pShape.setOutlineThickness(1);
        pShape.setPosition(pShapePos);
        window.draw(pShape);

        const float dirWidth = 0.010f * m_Size.x;
        const float dirLength = 0.030f * m_Size.x;

        // Draw player look direction
        sf::RectangleShape pDirShape({dirLength, dirWidth});
        pDirShape.setFillColor(sf::Color::Blue);
        pDirShape.setPosition(pShapePos);
        pDirShape.rotate(player->GetRotation() * Math::Rad2Deg);
        window.draw(pDirShape);
    }

    if (m_Flags & SHOW_LIGHTS)
    {
        const float lightWidth = 0.020f * m_Size.x;

        // Draw lights
        for (Light& light : level.GetLights())
        {
            sf::RectangleShape lightShape({lightWidth, lightWidth});
            lightShape.setFillColor(sf::Color::Yellow);
            lightShape.setOutlineColor(sf::Color::Black);
            lightShape.setOutlineThickness(1);
            lightShape.rotate(45);
            lightShape.setPosition({
                (light.position.x / level.GetSize().x) * m_Size.x + m_Origin.x,
                (light.position.y / level.GetSize().y) * m_Size.y + m_Origin.y
            });

            window.draw(lightShape);
        }
    }

    // Draw rays
    for (int i = -1; i <= 1; i++)
    {
        const float hitCircleRadius = 0.010f * m_Size.x;
        const float rayWidth = 0.010f * m_Size.x;

        Ray hitRay;

        if (m_Flags & SHOW_WALL_RAYS)
        {
            float angle = player->GetRotation() + (1 / Math::PI) * i;
            Ray::Cast(level, player->GetPosition(), angle, hitRay);

            // Draw ray direction
            sf::RectangleShape hitDirShape({(hitRay.distance / level.GetSize().x) * m_Size.x, rayWidth});
            hitDirShape.setFillColor(sf::Color::Blue);
            hitDirShape.setPosition(pShapePos);
            hitDirShape.rotate(angle * Math::Rad2Deg);
            window.draw(hitDirShape);

            // Draw ray hit
            sf::CircleShape hitCircle(hitCircleRadius);
            hitCircle.setOrigin(hitCircleRadius / 2, hitCircleRadius / 2);
            hitCircle.setFillColor(sf::Color::Red);
            hitCircle.setPosition({
                (hitRay.hitPos.x / level.GetSize().x) * m_Size.x + m_Origin.x,
                (hitRay.hitPos.y / level.GetSize().y) * m_Size.y + m_Origin.y
            });
            window.draw(hitCircle);
        }

        if (m_Flags & SHOW_LIGHT_RAYS)
        {
            for (Light& light : level.GetLights())
            {
                Ray lightRay;
                float angleToHit = Math::Vector2Angle(hitRay.hitPos - light.position);
                Ray::Cast(level, light.position, angleToHit, lightRay);

                // Draw light direction
                sf::RectangleShape lightDirShape({(lightRay.distance / level.GetSize().x) * m_Size.x, rayWidth});
                lightDirShape.setFillColor(sf::Color::Yellow);
                lightDirShape.setPosition({
                    (light.position.x / level.GetSize().x) * m_Size.x + m_Origin.x,
                    (light.position.y / level.GetSize().y) * m_Size.y + m_Origin.y
                });
                lightDirShape.rotate(angleToHit * Math::Rad2Deg);
                window.draw(lightDirShape);

                // Draw light ray hit
                sf::CircleShape lightHitCircle(hitCircleRadius);
                lightHitCircle.setOrigin(hitCircleRadius / 2, hitCircleRadius / 2);
                lightHitCircle.setFillColor(sf::Color::Red);
                sf::Vector2f lightHitPos = {
                    (lightRay.hitPos.x / level.GetSize().x) * m_Size.x,
                    (lightRay.hitPos.y / level.GetSize().y) * m_Size.y
                };
                lightHitCircle.setPosition(lightHitPos + m_Origin);
                window.draw(lightHitCircle);
            }
        }
    }
}

void MiniMap::Init()
{
    auto& level = Game::Get().GetCurrentLevel();
    m_Player = level.GetObject<Player>();
}

void MiniMap::Render(float dt)
{
    RenderMiniMap();
}