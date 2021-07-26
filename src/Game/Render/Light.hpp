#pragma once
#include <Rpch.hpp>

struct Light
{
    Light(sf::Vector2f position, float intensity, sf::Color color = sf::Color::White)
        : position(position), intensity(intensity), color(color) { }

    sf::Vector2f position;
    float intensity;
    sf::Color color;
};
