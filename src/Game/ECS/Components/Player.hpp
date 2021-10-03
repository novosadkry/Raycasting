#pragma once
#include <Core.hpp>

#include <Game/ECS/Component.hpp>

namespace ECS::Components
{
    struct Player : public Component
    {
        float speed;
    };

    template<typename Archive>
    void serialize(Archive& archive, Player& value)
    {
        archive(value.speed);
    }
}
