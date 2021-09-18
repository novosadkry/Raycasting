#pragma once
#include <Core.hpp>

struct Light
{
    Light() = default;
    Light(sf::Vector2f position, float intensity, sf::Color color = sf::Color::White)
        : position(position), intensity(intensity), color(color) { }

    sf::Vector2f position;
    float intensity;
    sf::Color color;
};
