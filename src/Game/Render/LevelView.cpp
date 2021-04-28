#include "LevelView.hpp"

#include <Game/Game.hpp>
#include "Ray.hpp"

static void RenderView(sf::RenderWindow& window, Level& level, Player& player, float fov, float wallCoeff, float depth)
{
    const float halfFov = fov / 2.0f;
    const sf::Vector2f windowSize {
        window.getView().getSize().x,
        window.getView().getSize().y
    };

    sf::RectangleShape stripe(sf::Vector2f(1, 1));

    for (unsigned int x = 0; x < windowSize.x; x++)
    {
        float angle = (player.GetDirection() - halfFov) + (x / windowSize.x) * fov;

        Ray hit;
        if (!Ray::Cast(level, player, angle, hit))
            continue;

        float ceiling = (windowSize.y / 2.0f) - (windowSize.y * wallCoeff / hit.distance);
        float floor = windowSize.y - ceiling;
        float wall = floor - ceiling;

        sf::Color wallColor = hit.hitSide.x
            ? sf::Color(220, 220, 220)
            : sf::Color::White;

        wallColor.r -= (sf::Uint8) (hit.distance / wallColor.r * depth);
        wallColor.g -= (sf::Uint8) (hit.distance / wallColor.g * depth);
        wallColor.b -= (sf::Uint8) (hit.distance / wallColor.b * depth);

        // sf::Vertex sCeiling[] =
        // {
        //     sf::Vertex(sf::Vector2f((float) x, 0), wallColor),
        //     sf::Vertex(sf::Vector2f((float) x, ceiling), sf::Color(50, 50, 50))
        // };

        // sf::Vertex sWall[] =
        // {
        //     sf::Vertex(sf::Vector2f((float) x, ceiling), wallColor),
        //     sf::Vertex(sf::Vector2f((float) x, ceiling + wall), wallColor)
        // };

        // sf::Vertex sFloor[] =
        // {
        //     sf::Vertex(sf::Vector2f((float) x, ceiling + wall), sf::Color(50, 50, 50)),
        //     sf::Vertex(sf::Vector2f((float) x, windowSize.y), wallColor)
        // };

        // window.draw(sCeiling, 2, sf::Lines);
        // window.draw(sWall, 2, sf::Lines);
        // window.draw(sFloor, 2, sf::Lines);

        stripe.setPosition((float) x, 0);
        stripe.setSize({1, ceiling});
        stripe.setFillColor(sf::Color::Yellow);
        window.draw(stripe);

        stripe.setPosition((float) x, ceiling);
        stripe.setSize({1, wall});
        stripe.setFillColor(wallColor);
        window.draw(stripe);

        stripe.setPosition((float) x, floor);
        stripe.setSize({1, floor});
        stripe.setFillColor(sf::Color::Green);
        window.draw(stripe);
    }
}

void LevelView::Render(float dt)
{
    RenderView(Game::Get().GetWindow(), Game::Get().GetCurrentLevel(), *m_Player, m_FOV, 50.0f, 50.0f);
}