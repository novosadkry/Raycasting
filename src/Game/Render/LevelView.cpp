#include "LevelView.hpp"

#include <Game/Game.hpp>
#include "Ray.hpp"

static void RenderView(sf::RenderWindow& window, Level& level, Player& player, float fov, float wallCoefficient)
{
    const float halfFov = fov / 2.0f;
    const sf::Vector2f windowSize {
        window.getView().getSize().x,
        window.getView().getSize().y
    };

    sf::RectangleShape stripe(sf::Vector2f(1, 1));
    stripe.setOrigin(0.5f, 0.5f);

    for (unsigned int x = 0; x < windowSize.x; x++)
    {
        float angle = (player.GetDirection() - halfFov) + (x / windowSize.x) * fov;

        Ray hit;
        if (!Ray::Cast(level, player, angle, hit))
            continue;

        float height = (windowSize.y * wallCoefficient) / hit.distance;

        stripe.setPosition((float) x, (windowSize.y / 2) - (height / 2));
        stripe.setSize({1, height});
        stripe.setFillColor(hit.hitSide.x ? sf::Color(220, 220, 220) : sf::Color::White);

        window.draw(stripe);
    }
}

void LevelView::Render(float dt)
{
    RenderView(Game::Get().GetWindow(), Game::Get().GetCurrentLevel(), *m_Player, m_FOV, 50.0f);
}