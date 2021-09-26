#pragma once
#include <Core.hpp>

#include <Game/ECS/Component.hpp>

struct Transform : public Component
{
    sf::Vector2f position;
};
