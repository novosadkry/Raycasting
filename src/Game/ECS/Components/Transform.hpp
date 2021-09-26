#pragma once
#include <Core.hpp>

#include <Game/ECS/Component.hpp>

namespace ECS::Components
{
    struct Transform : public Component
    {
        sf::Vector2f position;
    };
}
