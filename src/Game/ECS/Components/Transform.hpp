#pragma once
#include <Core.hpp>

#include <Game/ECS/Component.hpp>

namespace ECS::Components
{
    struct Transform : public Component
    {
        sf::Vector2f position;
        float rotation;
    };

    template<typename Archive>
    void serialize(Archive& archive, Transform& value)
    {
        archive(value.position, value.rotation);
    }
}
