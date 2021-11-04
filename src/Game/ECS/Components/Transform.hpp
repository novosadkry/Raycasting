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
            .COMP_DATA(&Transform::position, "position")
            .COMP_DATA(&Transform::rotation, "rotation")
            .COMP_TYPE(Transform);
    }

    template<typename Archive>
    void serialize(Archive& archive, Transform& value)
    {
        archive(value.position, value.rotation);
    }
}