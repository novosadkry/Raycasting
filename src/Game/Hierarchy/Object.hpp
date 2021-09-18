#pragma once
#include <Core.hpp>

class Object : public Serializable<Object>
{
public:
    Object() = default;
    virtual ~Object() = default;

    virtual void Init() { }
    virtual void Update(float dt) { }

    static Unique<Object> Deserialize(std::istream &stream);
};