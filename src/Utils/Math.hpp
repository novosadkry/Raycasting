#pragma once

#include <cmath>
#include <SFML/System/Vector2.hpp>

namespace Math
{
    const float PI = 3.141592653f;
    const float PI_2 = 2 * PI;

    const float Deg2Rad = PI / 180.0f;
    const float Rad2Deg = 180.0f / PI;

    inline sf::Vector2f Angle2Vector(float angle)
    {
        return sf::Vector2f(
            cos(angle),
            sin(angle)
        );
    }
}
