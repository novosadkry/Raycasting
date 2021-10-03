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

    template<typename Archive>
    void serialize(Archive& archive, Light& value)
    {
        archive(value.color, value.intensity);
    }
}
