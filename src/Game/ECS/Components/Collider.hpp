#pragma once
#include <Core.hpp>

#include <Game/ECS/Component.hpp>

namespace ECS::Components
{
    struct Collider : public Component
    {
        int sides;
        float radius;
    };
}
