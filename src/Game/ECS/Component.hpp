#pragma once
#include <Core.hpp>

#define COMP_TYPE(TYPE)                             \
    func<&Get<TYPE>, entt::as_ref_t>("get"_hs)      \
    .func<&Emplace<TYPE>>("emplace"_hs)             \
    .func<&Remove<TYPE>>("remove"_hs)               \
    .type()                                         \
    .props(                                         \
        std::make_pair("name"_hs, #TYPE),           \
        std::make_pair("type"_hs, "Component"_hs)   \
    )                                               \

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

    template<typename Component, typename... Args>
    void Emplace(entt::registry& registry, entt::entity entity, Args&&... args)
    {
        registry.emplace_or_replace<Component>(entity, std::forward<Args>(args)...);
    }

    template<typename... Component>
    void Remove(entt::registry& registry, entt::entity entity)
    {
        registry.remove<Component...>(entity);
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

    template<typename Func>
    void Types(Func func)
    {
        using namespace entt::literals;

        for (auto meta : entt::resolve())
        {
            if (auto type = meta.prop("type"_hs))
            {
                if (type.value() == "Component"_hs)
                    func(meta);
            }
        }
    }
}
