#pragma once
#include <Core.hpp>

class Object : public Serializable<Object>
{
public:
    Object() = default;
    virtual ~Object() = default;

    virtual void Update(float dt) { }
    virtual void Render(float dt) { }

    static Object Deserialize(std::istream &stream);
};