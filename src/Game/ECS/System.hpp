#pragma once
#include <Core.hpp>

namespace ECS
{
    class System
    {
    public:
        System(entt::registry* registry)
            : m_Registry(registry) { }

        virtual void Run(float dt) = 0;

    protected:
        entt::registry* m_Registry;
    };
}
