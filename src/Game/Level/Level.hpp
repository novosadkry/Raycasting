#pragma once

#include "../Grid/Grid.hpp"

class Level
{
public:
    Level(int index, const Grid& grid)
        : m_Index(index), m_Grid(grid) { }

    ~Level();

private:
    int m_Index;
    Grid m_Grid;
};