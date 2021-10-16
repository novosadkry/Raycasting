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
            .data<&Collider::sides>("sides"_hs)
            .data<&Collider::radius>("radius"_hs)
            .type();
    }

    template<typename Archive>
    void serialize(Archive& archive, Collider& value)
    {
        archive(value.sides, value.radius);
    }
}
