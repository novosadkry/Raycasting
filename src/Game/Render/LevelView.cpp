#include "LevelView.hpp"

#include <Game/Game.hpp>
#include <Game/Ray.hpp>
#include <Utils/Math.hpp>

// https://lodev.org/cgtutor/raycasting.html
// http://www.permadi.com/tutorial/raycast/rayc8.html
static void RenderView(sf::RenderWindow& window, Level& level, Player& player, sf::RenderTexture& buffer, Canvas& canvas, float wallCoeff, float depth)
{
    // Keep original resolution before downscaling
    const auto originalView = window.getView();

    // Downscale to desired resolution
    window.setView(sf::View(sf::FloatRect(0, 0, (float) buffer.getSize().x, (float) buffer.getSize().y)));
    const auto viewSize = window.getView().getSize();

    // Clear buffer before use
    buffer.clear();

    for (unsigned int screenX = 0; screenX < viewSize.x; screenX++)
    {
        float canvasX = (screenX / viewSize.x) * canvas.size;
        float angle = player.GetRotation() + atan((canvasX - canvas.size / 2) / canvas.distance);

        Ray hit;
        if (!Ray::Cast(level, player.GetPosition(), angle, hit))
            continue;

        // Correct the fishbowl effect
        hit.distance *= cos(player.GetRotation() - angle);

        float ceiling = (viewSize.y / 2.0f) - (viewSize.y * wallCoeff / hit.distance);
        float floor = viewSize.y - ceiling;
        float wall = floor - ceiling;

        sf::Color wallColor = hit.hitSide.x
            ? sf::Color(220, 220, 220)
            : sf::Color::White;

        wallColor.r -= (sf::Uint8) (hit.distance / wallColor.r * depth);
        wallColor.g -= (sf::Uint8) (hit.distance / wallColor.g * depth);
        wallColor.b -= (sf::Uint8) (hit.distance / wallColor.b * depth);

        sf::Vertex sCeiling[] =
        {
            sf::Vertex(sf::Vector2f(screenX + 0.5f, 0), wallColor),
            sf::Vertex(sf::Vector2f(screenX + 0.5f, ceiling), sf::Color(50, 50, 50))
        };

        sf::Vertex sWall[] =
        {
            sf::Vertex(sf::Vector2f(screenX + 0.5f, ceiling), wallColor),
            sf::Vertex(sf::Vector2f(screenX + 0.5f, ceiling + wall), wallColor)
        };

        sf::Vertex sFloor[] =
        {
            sf::Vertex(sf::Vector2f(screenX + 0.5f, ceiling + wall), sf::Color(50, 50, 50)),
            sf::Vertex(sf::Vector2f(screenX + 0.5f, viewSize.y), wallColor)
        };

        buffer.draw(sCeiling, 2, sf::Lines);
        buffer.draw(sWall, 2, sf::Lines);
        buffer.draw(sFloor, 2, sf::Lines);
    }

    buffer.display();

    window.draw(sf::Sprite(buffer.getTexture()));
    window.setView(originalView); // Upscale buffer to original resolution
}

void LevelView::Render(float dt)
{
    RenderView(Game::Get().GetWindow(), Game::Get().GetCurrentLevel(), *m_Player, *m_Buffer, m_Canvas, 50.0f, 50.0f);
}