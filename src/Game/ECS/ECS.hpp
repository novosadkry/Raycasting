#pragma once
#include <Core.hpp>

#include "Entity.hpp"
#include "System.hpp"
#include "Component.hpp"

#include "Systems/Collision.hpp"
#include "Systems/Controller.hpp"

#include "Components/Tag.hpp"
#include "Components/Light.hpp"
#include "Components/Player.hpp"
#include "Components/Collider.hpp"
#include "Components/Transform.hpp"

namespace ECS
{
    template<typename... Component>
    using ComponentGroup = entt::type_list<Component...>;

    using AllComponents = ComponentGroup
    <
        Components::Tag,
        Components::Light,
        Components::Player,
        Components::Collider,
        Components::Transform
    >;

    template<typename... Component>
    void Init(ComponentGroup<Component...>)
    {
        (Components::Register<Component>(), ...);
    }
}