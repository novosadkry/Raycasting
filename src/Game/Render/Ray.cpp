#include "Ray.hpp"
#include <Utils/Math.hpp>

bool Ray::Cast(Level& level, const sf::Vector2f& position, float angle, Ray& hit)
{
    sf::Vector2i cell = level.GetGridCellFromPos(position);
    sf::Vector2f cellSize = level.GetGrid().GetCellSize(level);

    sf::Vector2f dir = Math::Angle2Vector(angle);

    sf::Vector2i step;
    sf::Vector2f delta;
    sf::Vector2f ray;

    if (dir.x < 0)
    {
        step.x = -1;
        delta.x = -cellSize.x / cos(angle);
        ray.x = (position.x - cell.x * cellSize.x) / cellSize.x * delta.x;
    }

    else
    {
        step.x = 1;
        delta.x = cellSize.x / cos(angle);
        ray.x = ((cell.x + 1) * cellSize.x - position.x) / cellSize.x * delta.x;
    }

    if (dir.y < 0)
    {
        step.y = -1;
        delta.y = -cellSize.y / sin(angle);
        ray.y = (position.y - cell.y * cellSize.y) / cellSize.y * delta.y;
    }

    else
    {
        step.y = 1;
        delta.y = cellSize.y / sin(angle);
        ray.y = ((cell.y + 1) * cellSize.y - position.y) / cellSize.y * delta.y;
    }

    float distance = 0;
    sf::Vector2f normal;

    for (int i = 0; i < 128; i++)
    {
        distance = std::min(ray.x, ray.y);

        if (ray.x < ray.y)
        {
            cell.x += step.x;
            ray.x += delta.x;

            normal = dir.x < 0
                ? sf::Vector2f(  1, 0 )
                : sf::Vector2f( -1, 0 );
        }

        else
        {
            cell.y += step.y;
            ray.y += delta.y;

            normal = dir.y < 0
                ? sf::Vector2f( 0,  1 )
                : sf::Vector2f( 0, -1 );
        }

        if (level.GetGrid().Get(cell).type == Cell::Wall)
        {
            hit = Ray(distance, dir, normal, (distance * dir) + position, cell);
            return true;
        }
    }

    return false;
}