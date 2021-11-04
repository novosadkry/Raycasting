#pragma once
#include <Core.hpp>

#define COMP_TYPE(TYPE)                             \
    func<&Get<TYPE>, entt::as_ref_t>("get"_hs)      \
    .type()                                         \
    .prop(std::make_pair("name"_hs, #TYPE))         \

#define COMP_DATA(DATA, NAME)                       \
    data<DATA, entt::as_ref_t>(NAME##_hs)           \
    .prop(std::make_pair("name"_hs, NAME))          \

namespace ECS::Components
{
    template<typename>
    void Register() = delete;

    template<typename Component>
    Component& Get(entt::registry& registry, entt::entity entity)
    {
        return registry.get<Component>(entity);
    }

    template<typename Func>
    void Each(entt::registry& registry, entt::entity entity, Func func)
    {
        using namespace entt::literals;

        registry.visit(entity, [&](auto id) {
            auto type = entt::resolve(id);
            auto get = type.func("get"_hs);
            auto any = get.invoke({}, entt::forward_as_meta(registry), entity);
            func(type, any);
        });
    }
}
