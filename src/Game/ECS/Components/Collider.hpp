#pragma once
#include <Core.hpp>

#include <Game/ECS/Component.hpp>

namespace ECS::Components
{
    struct Collider
    {
        int sides;
        float radius;
    };

    template<>
    inline void Register<Collider>()
    {
        using namespace entt::literals;

        entt::meta<Collider>()
            .COMP_DATA(&Collider::sides, "sides")
            .COMP_DATA(&Collider::radius, "radius")
            .COMP_TYPE(Collider);
    }

    template<typename Archive>
    void serialize(Archive& archive, Collider& value)
    {
        archive(value.sides, value.radius);
    }
}
