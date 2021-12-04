#pragma once
#include <Core.hpp>

struct Canvas
{
    static Canvas From(sf::Vector2u size, float fov)
    {
        return Canvas {
            fov,
            size.x / 2.0f / tanf(fov / 2.0f),
            size
        };
    }

    float fov;
    float distance;
    sf::Vector2u size;
};
