#pragma once
#include <Rpch.hpp>

class Object
{
public:
    Object() = default;
    virtual ~Object() = default;

    virtual void Update(float dt) { }
    virtual void Render(float dt) { }
};