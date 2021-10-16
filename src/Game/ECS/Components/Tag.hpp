#pragma once
#include <Core.hpp>

#include <Game/ECS/Component.hpp>

namespace ECS::Components
{
    struct Tag
    {
        std::string name;
    };

    template<>
    inline void Register<Tag>()
    {
        using namespace entt::literals;

        entt::meta<Tag>()
            .data<&Tag::name>("name"_hs)
            .type();
    }

    template<typename Archive>
    void serialize(Archive& archive, Tag& value)
    {
        archive(value.name);
    }
}
