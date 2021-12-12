#pragma once
#include <Core.hpp>

#include <Game/ECS/Component.hpp>
#include <Game/Render/Canvas.hpp>

namespace ECS::Components
{
    struct Camera
    {
        Canvas canvas = Canvas::From(
            sf::Vector2u(300, 200),
            100.0f * Math::Deg2Rad
        );
    };

    template<>
    inline void Register<Camera>()
    {
        using namespace entt::literals;

        entt::meta<Camera>()
            .COMP_DATA(&Camera::canvas, "canvas")
            .COMP_TYPE(Camera);

        entt::meta<Canvas>()
            .COMP_DATA(&Canvas::fov, "fov")
            .COMP_DATA(&Canvas::distance, "distance")
            .COMP_DATA(&Canvas::size, "size")
            .type();
    }

    template<typename Archive>
    void serialize(Archive& archive, Camera& value)
    {
        archive(value.canvas);
    }
}
