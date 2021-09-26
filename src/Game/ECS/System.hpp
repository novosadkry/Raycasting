#pragma once
#include <Core.hpp>

namespace ECS
{
    class System
    {
        virtual void Run(float dt) = 0;
    };
}
