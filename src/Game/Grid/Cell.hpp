#pragma once
#include <Core.hpp>

struct Cell
{
    enum Type : uint8_t
    {
        Empty,
        Wall
    };

    Cell(Type type = (Type)0)
        : type(type), texture(0) { }

    Type type : 4;
    uint8_t texture : 4;
};