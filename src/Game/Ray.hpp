#pragma once
#include <Rpch.hpp>

#include <Game/Level.hpp>
#include <Game/Render/Player.hpp>

struct Ray
{
public:
    static bool Cast(Level& level, const sf::Vector2f& position, float angle, Ray& hit);

public:
    Ray()
        { }

    Ray(float distance, const sf::Vector2f& dir, const sf::Vector2f& normal, const sf::Vector2f& hitPos)
        : distance(distance), dir(dir), normal(normal), hitPos(hitPos) { }

    float distance;
    sf::Vector2f dir;
    sf::Vector2f normal;
    sf::Vector2f hitPos;
};