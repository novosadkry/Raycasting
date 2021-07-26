#pragma once
#include <Rpch.hpp>

class Object
{
public:
    Object() { }

    virtual void Update(float dt) { }
    virtual void Render(float dt) { }
};