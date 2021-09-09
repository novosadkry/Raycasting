#pragma once
#include <Core.hpp>

class Layer
{
public:
    Layer() = default;
    virtual ~Layer() = default;

    virtual void Init() { }
    virtual void Render(float dt) { }
};
