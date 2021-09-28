#pragma once
#include <Core.hpp>

#include <Game/ECS/Component.hpp>

namespace ECS::Components
{
    struct Light : public Component
    {
        sf::Color color;
        float intensity;
    };
}