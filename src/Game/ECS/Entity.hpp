#pragma once
#include <Core.hpp>

#include "Component.hpp"

namespace ECS
{
    class Entity
    {
    public:
        Entity() = default;

        Entity(entt::entity handle, entt::registry* registry)
            : m_Handle(handle), m_Registry(registry) { }

        template<typename T, typename... Args>
        void Add(Args&&... args)
        {
            m_Registry->emplace<T>(m_Handle, Component{}, std::forward<Args>(args)...);
        }

        template<typename... T>
        auto& Get()
        {
            return m_Registry->get<T...>(m_Handle);
        }

        template<typename... T>
        void Remove()
        {
            return m_Registry->remove<T...>(m_Handle);
        }

        inline operator bool() const
        {
            return m_Handle != entt::null;
        }

        inline operator entt::entity() const
        {
            return m_Handle;
        }

    private:
        entt::entity m_Handle = entt::null;
        entt::registry* m_Registry;
    };
}
