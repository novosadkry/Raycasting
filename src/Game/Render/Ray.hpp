#pragma once

#include <Game/Level.hpp>
#include "Player.hpp"

struct Ray
{
public:
    static bool Cast(Level& level, Player& player, float angle, Ray& hit);

public:
    Ray()
        { }

    Ray(float distance, const sf::Vector2f& dir, const sf::Vector2i& hitSide)
        : distance(distance), dir(dir), hitSide(hitSide) { }

    float distance;
    sf::Vector2f dir;
    sf::Vector2i hitSide;
};