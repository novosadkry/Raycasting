#include "Object.hpp"

Object Object::Deserialize(std::istream &stream)
{
    return ::Deserialize<Object>("Player", stream);
}