#include "Object.hpp"

Unique<Object> Object::Deserialize(std::istream &stream)
{
    return ::Deserialize<Object>("Player", stream);
}