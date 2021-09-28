#pragma once
#include <Core.hpp>

#include <Game/ECS/Entity.hpp>

namespace ECS
{
    class Hierarchy
    {
    public:
        template<typename... T>
        ECS::Entity GetEntity()
        {
            return ECS::Entity(m_Registry.view<T...>().front(), &m_Registry);
        }

        template<typename... T>
        std::vector<ECS::Entity> GetEntities()
        {
            std::vector<ECS::Entity> result;
            m_Registry.view<T...>().each([&](auto& entity)
            {
                result.emplace_back(entity, &m_Registry);
            });

            return result;
        }

        inline ECS::Entity CreateEntity()
        {
            return ECS::Entity(m_Registry.create(), &m_Registry);
        }

        inline void DestroyEntity(ECS::Entity& entity)
        {
            m_Registry.destroy(entity);
        }

        inline entt::registry& GetRegistry()
        {
            return m_Registry;
        }

    private:
        entt::registry m_Registry;
    };
}
