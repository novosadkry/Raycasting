#include "LevelView.hpp"

#include <Game/Game.hpp>
#include <Game/Render/Ray.hpp>

#include <Game/ECS/Components/Light.hpp>
#include <Game/ECS/Components/Player.hpp>
#include <Game/ECS/Components/Transform.hpp>

static sf::Color CalculateLight(Ray& hit, float depth)
{
    auto& level = Game::Get().GetCurrentLevel();
    auto lights = Game::Get()
        .GetCurrentLevel()
        .GetHierarchy()
        .GetRegistry()
        .group<ECS::Components::Light>(entt::get<ECS::Components::Transform>);

    float r, g, b;
    r = g = b = 0;

    lights.each([&](ECS::Components::Light& light, ECS::Components::Transform& transform)
    {
        Ray lightRay;
        float distToHit = Math::DistanceSqr(hit.hitPos, transform.position);
        float angleToHit = Math::Vector2Angle(hit.hitPos - transform.position);

        if (Ray::Cast(level, transform.position, angleToHit, lightRay))
        {
            // Squared distance to save computation time
            if (abs(lightRay.distance * lightRay.distance - distToHit) < 0.5f)
            {
                float w = fmax(1 - (lightRay.distance / (depth * light.intensity)), 0);

                r += w * light.color.r;
                g += w * light.color.g;
                b += w * light.color.b;
            }
        }
    });

    r = fmin(r, 255);
    g = fmin(g, 255);
    b = fmin(b, 255);

    return sf::Color(r, g, b);
}

// https://lodev.org/cgtutor/raycasting.html
// http://www.permadi.com/tutorial/raycast/rayc8.html
void LevelView::RenderView()
{
    const float wallCoeff = 50.0f;
    const float depth     = 300.0f;

    auto& window = Game::Get().GetWindow();
    auto& level  = Game::Get().GetCurrentLevel();

    if (!m_Player) return;
    auto player = m_Player.Get<ECS::Components::Transform>();

    // Keep original resolution before downscaling
    const auto originalView = window.getView();

    // Downscale to desired resolution
    window.setView(sf::View(sf::FloatRect(0, 0, (float) m_Buffer->getSize().x, (float) m_Buffer->getSize().y)));
    const auto viewSize = window.getView().getSize();

    // Clear buffer before use
    m_Buffer->clear();

    for (unsigned int screenX = 0; screenX < viewSize.x; screenX++)
    {
        float canvasX = (screenX / viewSize.x) * m_Canvas.size;
        float angle = player.rotation + atan((canvasX - m_Canvas.size / 2) / m_Canvas.distance);

        Ray hit;
        if (!Ray::Cast(level, player.position, angle, hit))
            continue;

        sf::Color wallColor = CalculateLight(hit, depth);

        // Correct the fishbowl effect
        hit.distance *= cos(player.rotation - angle);

        float ceiling = (viewSize.y / 2.0f) - (viewSize.y * wallCoeff / hit.distance);
        float floor = viewSize.y - ceiling;
        float wall = floor - ceiling;

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

        m_Buffer->draw(sCeiling, 2, sf::Lines);
        m_Buffer->draw(sWall, 2, sf::Lines);
        m_Buffer->draw(sFloor, 2, sf::Lines);
    }

    m_Buffer->display();

    window.draw(sf::Sprite(m_Buffer->getTexture()));
    window.setView(originalView); // Upscale buffer to original resolution
}

void LevelView::Init()
{
    m_Player = Game::Get()
        .GetCurrentLevel()
        .GetHierarchy()
        .GetEntity<ECS::Components::Player>();
}

void LevelView::Render(float dt)
{
    RenderView();
}