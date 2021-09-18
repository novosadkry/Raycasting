#include "Object.hpp"

Unique<Object> Object::Deserialize(std::istream &stream)
{
    std::string type;
    stream >> type;

    return ::Deserialize<Object>(type, stream);
}