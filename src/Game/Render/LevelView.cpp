#include "LevelView.hpp"

#include <Game/Game.hpp>
#include <Game/Ray.hpp>
#include <Utils/Math.hpp>

// https://lodev.org/cgtutor/raycasting.html
// http://www.permadi.com/tutorial/raycast/rayc8.html
static void RenderView(sf::RenderWindow& window, Level& level, Player& player, const Canvas& canvas, float wallCoeff, float depth)
{
    const float halfFov = canvas.fov / 2.0f;
    const sf::Vector2f windowSize {
        window.getView().getSize().x,
        window.getView().getSize().y
    };

    // sf::RectangleShape stripe(sf::Vector2f(1, 1));

    for (unsigned int screenX = 0; screenX < windowSize.x; screenX++)
    {
        float canvasX = (screenX / windowSize.x) * canvas.size;
        float angle = player.GetRotation() + atan((canvasX - canvas.size / 2) / canvas.distance);

        Ray hit;
        if (!Ray::Cast(level, player.GetPosition(), angle, hit))
            continue;

        // Correct the fishbowl effect
        hit.distance *= cos(player.GetRotation() - angle);

        float ceiling = (windowSize.y / 2.0f) - (windowSize.y * wallCoeff / hit.distance);
        float floor = windowSize.y - ceiling;
        float wall = floor - ceiling;

        sf::Color wallColor = hit.hitSide.x
            ? sf::Color(220, 220, 220)
            : sf::Color::White;

        wallColor.r -= (sf::Uint8) (hit.distance / wallColor.r * depth);
        wallColor.g -= (sf::Uint8) (hit.distance / wallColor.g * depth);
        wallColor.b -= (sf::Uint8) (hit.distance / wallColor.b * depth);

        sf::Vertex sCeiling[] =
        {
            sf::Vertex(sf::Vector2f((float) screenX, 0), wallColor),
            sf::Vertex(sf::Vector2f((float) screenX, ceiling), sf::Color(50, 50, 50))
        };

        sf::Vertex sWall[] =
        {
            sf::Vertex(sf::Vector2f((float) screenX, ceiling), wallColor),
            sf::Vertex(sf::Vector2f((float) screenX, ceiling + wall), wallColor)
        };

        sf::Vertex sFloor[] =
        {
            sf::Vertex(sf::Vector2f((float) screenX, ceiling + wall), sf::Color(50, 50, 50)),
            sf::Vertex(sf::Vector2f((float) screenX, windowSize.y), wallColor)
        };

        window.draw(sCeiling, 2, sf::Lines);
        window.draw(sWall, 2, sf::Lines);
        window.draw(sFloor, 2, sf::Lines);

        // stripe.setPosition((float) screenX, 0);
        // stripe.setSize({1, ceiling});
        // stripe.setFillColor(sf::Color::Yellow);
        // window.draw(stripe);

        // stripe.setPosition((float) screenX, ceiling);
        // stripe.setSize({1, wall});
        // stripe.setFillColor(wallColor);
        // window.draw(stripe);

        // stripe.setPosition((float) screenX, floor);
        // stripe.setSize({1, floor});
        // stripe.setFillColor(sf::Color::Green);
        // window.draw(stripe);
    }
}

void LevelView::Render(float dt)
{
    RenderView(Game::Get().GetWindow(), Game::Get().GetCurrentLevel(), *m_Player, m_Canvas, 50.0f, 50.0f);
}