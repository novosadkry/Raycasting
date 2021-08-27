#pragma once
#include <Core.hpp>

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

    inline float Vector2Angle(const sf::Vector2f& v)
    {
        return atan2(v.y, v.x);
    }

    inline float Length(const sf::Vector2f& v)
    {
        return sqrt(v.x * v.x + v.y * v.y);
    }

    inline float LengthSqr(const sf::Vector2f& v)
    {
        return v.x * v.x + v.y * v.y;
    }

    inline float Distance(const sf::Vector2f& v1, const sf::Vector2f& v2)
    {
        return Length(v1 - v2);
    }

    inline float DistanceSqr(const sf::Vector2f& v1, const sf::Vector2f& v2)
    {
        return LengthSqr(v1 - v2);
    }

    inline void Normalize(sf::Vector2f& v)
    {
        float length = Length(v);
        v.x /= length;
        v.y /= length;
    }

    inline float Dot(const sf::Vector2f& v1, const sf::Vector2f& v2)
    {
        return v1.x * v2.x + v1.y * v2.y;
    }

    inline sf::Vector2f Reflect(const sf::Vector2f& v, const sf::Vector2f& n)
    {
        return v - 2.0f * Dot(n, v) * n;
    }
}
