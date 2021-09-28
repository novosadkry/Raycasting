#pragma once
#include <Core.hpp>

#include <Game/ECS/System.hpp>

namespace ECS::Systems
{
    class Controller : public System
    {
    public:
        Controller(entt::registry* registry)
            : System(registry) { }

        void Update(float dt) override;
    };
}
