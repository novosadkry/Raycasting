#pragma once
#include <Core.hpp>

#include <Game/ECS/System.hpp>

namespace ECS::Systems
{
    class Collision : public System
    {
    public:
        Collision(entt::registry* registry)
            : System(registry) { }

        void Update(float dt) override;
    };
}
