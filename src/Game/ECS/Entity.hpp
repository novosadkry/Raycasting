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
            m_Registry->emplace<T>(m_Handle, std::forward<Args>(args)...);
        }

        template<typename... T>
        auto& Get()
        {
            return m_Registry->get<T...>(m_Handle);
        }

        template<typename T>
        auto TryGet()
        {
            return m_Registry->try_get<T>(m_Handle);
        }

        template<typename T>
        auto Has()
        {
            return !!m_Registry->try_get<T>(m_Handle);
        }

        template<typename... T>
        void Remove()
        {
            return m_Registry->remove<T...>(m_Handle);
        }

        template<typename Func>
        void Each(Func func)
        {
            Components::Each(*m_Registry, *this, func);
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
