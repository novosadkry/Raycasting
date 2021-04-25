#pragma once

#include <cstdint>

enum class CellType : uint8_t
{
    Empty,
    Wall
};

struct Cell
{
    CellType type;
};