#pragma once
#include <Core.hpp>

#include <Game/ECS/Component.hpp>

namespace ECS::Components
{
    struct Light
    {
        sf::Color color;
        float intensity;
    };

    template<>
    inline void Register<Light>()
    {
        using namespace entt::literals;

        entt::meta<Light>()
            .COMP_DATA(&Light::color, "color")
            .COMP_DATA(&Light::intensity, "intensity")
            .COMP_TYPE(Light);
    }

    template<typename Archive>
    void serialize(Archive& archive, Light& value)
    {
        archive(value.color, value.intensity);
    }
}
