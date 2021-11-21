#pragma once
#include <Core.hpp>

namespace ECS
{
    class System
    {
    public:
        System(entt::registry* registry)
            : m_Registry(registry) { }

        virtual ~System() = default;

        virtual void OnLoad() { }
        virtual void OnUnload() { }
        virtual void Update(float dt) = 0;

    protected:
        entt::registry* m_Registry;
    };

    class SystemGroup
    {
    public:
        SystemGroup(entt::registry* registry)
            : m_Registry(registry) { }

        template<typename T>
        void Add()
        {
            m_Systems.emplace_back(MakeUnique<T>(m_Registry));
        }

        template<typename T>
        void Remove()
        {
            for (auto it = m_Systems.begin(); it != m_Systems.end(); it++)
            {
                if (dynamic_cast<T*>(it->get()))
                {
                    it = m_Systems.erase(it);
                    return;
                }
            }
        }

        void Update(float dt)
        {
            for (auto& system : m_Systems)
                system->Update(dt);
        }

    private:
        entt::registry* m_Registry;
        std::vector<Unique<System>> m_Systems;
    };
}
