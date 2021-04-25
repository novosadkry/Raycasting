#pragma once

#include "Cell.hpp"
#include <vector>

class Grid
{
public:
    Grid(int x, int y)
        : m_X(x), m_Y(y)
    {
        m_Cells = std::vector<Cell>(x * y);
    }

    Cell& Get(int x, int y);
    void Set(int x, int y, const Cell& cell);

private:
    int m_X, m_Y;
    std::vector<Cell> m_Cells;
};