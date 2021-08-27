#pragma once
#include <Core.hpp>

template<typename T>
class Serializable
{
public:
    virtual void Serialize(std::ostream& stream) = 0;
    static Unique<T> Deserialize(std::istream& stream);
};
