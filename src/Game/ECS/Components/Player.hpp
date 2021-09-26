#pragma once
#include <Core.hpp>

#include <Game/ECS/Component.hpp>

namespace ECS::Components
{
    struct Player : public Component
    {
        float speed;
    };
}