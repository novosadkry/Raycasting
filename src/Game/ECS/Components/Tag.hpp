#pragma once
#include <Core.hpp>

#include <Game/ECS/Component.hpp>

namespace ECS::Components
{
    struct Tag : public Component
    {
        std::string name;
    };

    template<typename Archive>
    void serialize(Archive& archive, Tag& value)
    {
        archive(value.name);
    }
}
