#pragma once
#include <Core.hpp>

#include <Game/ECS/Component.hpp>

struct Tag : public Component
{
    std::string name;
};
