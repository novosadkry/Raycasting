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

    template<typename Archive>
    void serialize(Archive& archive, Collider& value)
    {
        archive(value.sides, value.radius);
    }
}
