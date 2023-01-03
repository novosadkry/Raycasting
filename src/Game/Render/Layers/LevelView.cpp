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
        .view<Camera, Transform>();

    for (auto e : view)
    {
        auto& cam = view.get<Transform>(e);
        auto& canvas = view.get<Camera>(e).canvas;

        if (!m_Buffer || canvas.size != m_Buffer->getSize())
        {
            m_Buffer = MakeUnique<sf::RenderTexture>();

            if (!m_Buffer->create(canvas.size.x, canvas.size.y))
                LOG("Unable to create RenderTexture!");
        }

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
            float canvasX = (screenX / viewSize.x) * canvas.size.x;
            float angle = cam.rotation + atan((canvasX - canvas.size.x / 2) / canvas.distance);

            Ray hit; std::vector<Ray::RayPass> passes;
            if (!Ray::Cast(level, cam.position, angle, hit, &passes))
                continue;

            Cell cell = level.GetGrid().Get(hit.cellPos);

            // Correct the fishbowl effect
            hit.distance *= cos(cam.rotation - angle);

            sf::Color wallColor = CalculateLight(hit, depth);

            float ceiling = (viewSize.y / 2.0f) - (canvas.distance * wallY / hit.distance);
            float floor = viewSize.y - ceiling;
            float wall = floor - ceiling;

            // Render floor and ceiling
            for (auto it = passes.begin(); it != passes.end() - 1; it++) {
                auto pass = *it;
                float canvasC = canvas.size.y / 2.0f;

                float distance[2] = {
                    Math::Distance(cam.position, pass.first),
                    Math::Distance(cam.position, pass.second)
                };

                // Correct the fishbowl effect
                distance[0] *= cos(cam.rotation - angle);
                distance[1] *= cos(cam.rotation - angle);

                float canvasY[2] = {
                    (canvas.distance * wallY) / distance[0] + canvasC,
                    (canvas.distance * wallY) / distance[1] + canvasC,
                };

                float screenY[2] = {
                    (viewSize.y * canvasY[0]) / canvas.size.y,
                    (viewSize.y * canvasY[1]) / canvas.size.y,
                };

                Cell cell = level.GetGrid().Get(level.GetGridCellFromPos(pass.first));

                sf::Vector2f cellSize = level.GetGrid().GetCellSize(level);
                Texture* texture = level.GetResources().Get<Texture>(cell.texture);

                sf::Color floorColor = sf::Color::White;

                sf::RenderStates states;
                sf::Vector2f tx[2];

                if (texture)
                {
                    tx[0].x = (fmod(pass.first.x, cellSize.x) / cellSize.x) * texture->GetSize().x;
                    tx[0].y = (fmod(pass.first.y, cellSize.y) / cellSize.y) * texture->GetSize().y;

                    tx[1].x = (fmod(pass.second.x, cellSize.x) / cellSize.x) * texture->GetSize().x;
                    tx[1].y = (fmod(pass.second.y, cellSize.y) / cellSize.y) * texture->GetSize().y;

                    states.texture = &texture->GetHandle();
                }

                sf::Vertex sFloor[] =
                {
                    sf::Vertex(sf::Vector2f(screenX, screenY[0]), floorColor, tx[0]),
                    sf::Vertex(sf::Vector2f(screenX, screenY[1]), floorColor, tx[1])
                };

                sf::Vertex sCeil[] =
                {
                    sf::Vertex(sf::Vector2f(screenX, viewSize.y - screenY[0]), floorColor, tx[0]),
                    sf::Vertex(sf::Vector2f(screenX, viewSize.y - screenY[1]), floorColor, tx[1])
                };

                m_Buffer->draw(sFloor, 2, sf::Lines, states);
                m_Buffer->draw(sCeil, 2, sf::Lines, states);
            }

            /* old code
            // Render floor and ceiling
            for (unsigned int screenY = ceiling + wall; screenY <= viewSize.y; screenY++)
            {
                float canvasC = canvas.size.y / 2.0f;
                float canvasY = (screenY / viewSize.y) * canvas.size.y;

                float distance = (canvas.distance * wallY) / (canvasY - canvasC);

                // Correct the fishbowl effect
                distance /= cos(cam.rotation - angle);

                auto position = cam.position + Math::Angle2Vector(angle) * distance;
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
            */

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