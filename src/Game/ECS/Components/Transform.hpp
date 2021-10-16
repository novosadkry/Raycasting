#pragma once
#include <Core.hpp>

#include <Game/ECS/Component.hpp>

namespace ECS::Components
{
    struct Transform
    {
        sf::Vector2f position;
        float rotation;
    };

    template<>
    inline void Register<Transform>()
    {
        using namespace entt::literals;

        entt::meta<Transform>()
            .data<&Transform::position>("position"_hs)
            .data<&Transform::rotation>("rotation"_hs)
            .type();
    }

    template<typename Archive>
    void serialize(Archive& archive, Transform& value)
    {
        archive(value.position, value.rotation);
    }
}