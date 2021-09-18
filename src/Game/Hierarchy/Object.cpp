#include "Object.hpp"

Unique<Object> Object::Deserialize(std::istream &stream)
{
    std::string type(::Deserialize<char[]>(stream).get());
    return ::Deserialize<Object>(type, stream);
}