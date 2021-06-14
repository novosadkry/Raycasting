#pragma once

#include <Game/Level.hpp>
#include <Game/Render/Player.hpp>

struct Ray
{
public:
    static bool Cast(Level& level, const sf::Vector2f& position, float angle, Ray& hit);

public:
    Ray()
        { }

    Ray(float distance, const sf::Vector2f& dir, const sf::Vector2i& hitSide, const sf::Vector2f& hitPos)
        : distance(distance), dir(dir), hitSide(hitSide), hitPos(hitPos) { }

    float distance;
    sf::Vector2f dir;
    sf::Vector2i hitSide;
    sf::Vector2f hitPos;
};