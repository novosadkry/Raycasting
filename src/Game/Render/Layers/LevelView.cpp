#include "LevelView.hpp"

#include <Game/Game.hpp>
#include <Game/ECS/ECS.hpp>
#include <Game/Render/Ray.hpp>
#include <Game/Render/Texture.hpp>

static sf::Color CalculateLight(Ray& hit, float depth)
{
    using namespace ECS::Components;

    auto& level = Game::Get().GetCurrentLevel();
    auto lights = Game::Get()
        .GetCurrentLevel()
        .GetHierarchy()
        .GetRegistry()
        .group<Light>(entt::get<Transform>);

    float r, g, b;
    r = g = b = 0;

    lights.each([&](Light& light, Transform& transform)
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
    using namespace ECS::Components;

    const float wallY = 50.0f;
    const float depth = 300.0f;

    auto& window = Game::Get().GetWindow();
    auto& level  = Game::Get().GetCurrentLevel();

    auto view = level
        .GetHierarchy()
        .GetRegistry()
        .view<Player, Transform>();

    for (auto e : view)
    {
        auto& player = view.get<Transform>(e);

        // Keep original resolution before downscaling
        const auto originalView = window.getView();

        // Downscale to desired resolution
        window.setView(sf::View(
            sf::FloatRect(0, 0,
            (float) m_Buffer->getSize().x,
            (float) m_Buffer->getSize().y)
        ));

        const auto viewSize = window.getView().getSize();

        // Clear buffer before use
        m_Buffer->clear();

        // Render walls
        for (unsigned int screenX = 0; screenX <= viewSize.x; screenX++)
        {
            float canvasX = (screenX / viewSize.x) * m_Canvas.size.x;
            float angle = player.rotation + atan((canvasX - m_Canvas.size.x / 2) / m_Canvas.distance);

            Ray hit;
            if (!Ray::Cast(level, player.position, angle, hit))
                continue;

            Cell cell = level.GetGrid().Get(hit.cellPos);

            // Correct the fishbowl effect
            hit.distance *= cos(player.rotation - angle);

            sf::Color wallColor = CalculateLight(hit, depth);

            float ceiling = (viewSize.y / 2.0f) - (m_Canvas.distance * wallY / hit.distance);
            float floor = viewSize.y - ceiling;
            float wall = floor - ceiling;

            // Render floor and ceiling
            for (unsigned int screenY = ceiling + wall; screenY <= viewSize.y; screenY++)
            {
                float canvasC = m_Canvas.size.y / 2.0f;
                float canvasY = (screenY / viewSize.y) * m_Canvas.size.y;

                float distance = (m_Canvas.distance * wallY) / (canvasY - canvasC);

                // Correct the fishbowl effect
                distance /= cos(player.rotation - angle);

                auto position = player.position + Math::Angle2Vector(angle) * distance;
                Cell cell = level.GetGrid().Get(level.GetGridCellFromPos(position));

                sf::Color floorColor = sf::Color::White;

                sf::Vector2f cellSize = level.GetGrid().GetCellSize(level);
                Texture* texture = level.GetResources().Get<Texture>(cell.texture);

                sf::RenderStates states;
                sf::Vector2f textureCoord;

                if (texture)
                {
                    textureCoord.x = (fmod(position.x, cellSize.x) / cellSize.x) * texture->GetSize().x;
                    textureCoord.y = (fmod(position.y, cellSize.y) / cellSize.y) * texture->GetSize().y;

                    states.texture = &texture->GetHandle();
                }

                sf::Vertex sFloor[] =
                {
                    sf::Vertex(sf::Vector2f(screenX, screenY), floorColor, textureCoord),
                    sf::Vertex(sf::Vector2f(screenX, viewSize.y - screenY), floorColor, textureCoord)
                };

                m_Buffer->draw(sFloor, 2, sf::Points, states);
            }

            sf::Vector2f cellSize = level.GetGrid().GetCellSize(level);
            Texture* texture = level.GetResources().Get<Texture>(cell.texture);

            sf::RenderStates states;
            sf::Vector2f textureCoord;

            if (texture)
            {
                if (hit.normal.x)
                {
                    textureCoord.y = texture->GetSize().y;
                    textureCoord.x = (fmod(hit.hitPos.y, cellSize.y) / cellSize.y) * texture->GetSize().x;
                }

                else
                {
                    textureCoord.y = texture->GetSize().y;
                    textureCoord.x = (fmod(hit.hitPos.x, cellSize.x) / cellSize.x) * texture->GetSize().x;
                }

                states.texture = &texture->GetHandle();
            }

            sf::Vertex sWall[] =
            {
                sf::Vertex(sf::Vector2f(screenX + 0.5f, ceiling), wallColor, sf::Vector2f(textureCoord.x, 0)),
                sf::Vertex(sf::Vector2f(screenX + 0.5f, ceiling + wall), wallColor, textureCoord)
            };

            m_Buffer->draw(sWall, 2, sf::Lines, states);
        }

        m_Buffer->display();

        window.draw(sf::Sprite(m_Buffer->getTexture()));
        window.setView(originalView); // Upscale buffer to original resolution
    }
}

void LevelView::Init()
{

}

void LevelView::Render(float dt)
{
    RenderView();
}