#pragma once

#include <memory>
#include <SFML/System/Clock.hpp>

class Object
{
public:
    Object() { }

    virtual void Update(float dt) { }
    virtual void Render(float dt) { }
};