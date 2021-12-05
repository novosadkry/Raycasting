#pragma once
#include <Core.hpp>

#include <Game/Level.hpp>

struct Ray
{
public:
    static bool Cast(Level& level, const sf::Vector2f& position, float angle, Ray& hit);

public:
    Ray() = default;

    Ray(float distance, sf::Vector2f dir, sf::Vector2f normal, sf::Vector2f hitPos, sf::Vector2i cellPos)
        : distance(distance), dir(dir), normal(normal), hitPos(hitPos), cellPos(cellPos) { }

    float distance;
    sf::Vector2f dir;
    sf::Vector2f normal;
    sf::Vector2f hitPos;
    sf::Vector2i cellPos;
};