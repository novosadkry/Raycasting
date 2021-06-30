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
            cosf(angle),
            sinf(angle)
        );
    }

    inline float Vector2Angle(sf::Vector2f vector)
    {
        return atan2f(vector.y, vector.x);
    }

    inline float Length(const sf::Vector2f& vector)
    {
        return sqrtf(vector.x * vector.x + vector.y * vector.y);
    }

    inline float LengthSqr(const sf::Vector2f& vector)
    {
        return vector.x * vector.x + vector.y * vector.y;
    }

    inline float Distance(const sf::Vector2f& v1, const sf::Vector2f& v2)
    {
        return Length(v1 - v2);
    }

    inline float DistanceSqr(const sf::Vector2f& v1, const sf::Vector2f& v2)
    {
        return LengthSqr(v1 - v2);
    }

    inline void Normalize(sf::Vector2f& vector)
    {
        float length = Length(vector);
        vector.x /= length;
        vector.y /= length;
    }
}
