#include "Ray.hpp"
#include <Utils/Math.hpp>

bool Ray::Cast(Level& level, Player& player, float angle, Ray& hit)
{
    angle = angle / 180.0f * PI;

    sf::Vector2i cell = level.GetGridCellFromPos(player.GetPosition());

    sf::Vector2f cellSize {
        (float) level.GetSize().x / level.GetGrid().SizeX(),
        (float) level.GetSize().y / level.GetGrid().SizeY()
    };

    sf::Vector2f dir {
        cos(angle),
        sin(angle)
    };

    sf::Vector2i step;
    sf::Vector2f delta;
    sf::Vector2f ray;

    if (dir.x < 0)
    {
        step.x = -1;
        delta.x = -cellSize.x / cos(angle);
        ray.x = (player.GetPosition().x - cell.x * cellSize.x) / cellSize.x * delta.x;
    }

    else
    {
        step.x = 1;
        delta.x = cellSize.x / cos(angle);
        ray.x = ((cell.x + 1) * cellSize.x - player.GetPosition().x) / cellSize.x * delta.x;
    }

    if (dir.y < 0)
    {
        step.y = -1;
        delta.y = -cellSize.y / sin(angle);
        ray.y = (player.GetPosition().y - cell.y * cellSize.y) / cellSize.y * delta.y;
    }

    else
    {
        step.y = 1;
        delta.y = cellSize.y / sin(angle);
        ray.y = ((cell.y + 1) * cellSize.y - player.GetPosition().y) / cellSize.y * delta.y;
    }

    float distance = 0;
    sf::Vector2i hitSide{};

    for (int i = 0; i < 128; i++)
    {
        distance = std::min(ray.x, ray.y);

        if (ray.x < ray.y)
        {
            cell.x += step.x;
            ray.x += delta.x;

            hitSide = { 1, 0 };
        }

        else
        {
            cell.y += step.y;
            ray.y += delta.y;

            hitSide = { 0, 1 };
        }

        if (level.GetGrid().Get(cell) == Wall)
        {
            hit = Ray(distance, dir, hitSide);
            return true;
        }
    }

    return false;
}