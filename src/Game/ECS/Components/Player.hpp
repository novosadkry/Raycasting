#pragma once
#include <Core.hpp>

#include <Game/ECS/Component.hpp>

namespace ECS::Components
{
    struct Player
    {
        float speed;
        float height;
    };

    template<>
    inline void Register<Player>()
    {
       using namespace entt::literals;

        entt::meta<Player>()
            .COMP_DATA(&Player::speed, "speed")
            .COMP_DATA(&Player::height, "height")
            .COMP_TYPE(Player);
    }

    template<typename Archive>
    void serialize(Archive& archive, Player& value)
    {
        archive(value.speed, value.height);
    }
}
