#pragma once
#include <Core.hpp>

#include <Game/ECS/System.hpp>

namespace ECS::Systems
{
    class Player : public System
    {
    public:
        void Run(float dt) override;
    };
}
