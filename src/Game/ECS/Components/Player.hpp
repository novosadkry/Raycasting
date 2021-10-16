#pragma once
#include <Core.hpp>

#include <Game/ECS/Component.hpp>

namespace ECS::Components
{
    struct Player
    {
        float speed;
    };

    template<>
    inline void Register<Player>()
    {
       using namespace entt::literals;

        entt::meta<Player>()
            .data<&Player::speed>("speed"_hs)
            .type();
    }

    template<typename Archive>
    void serialize(Archive& archive, Player& value)
    {
        archive(value.speed);
    }
}
