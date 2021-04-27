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

    Grid(Grid&& other)
        : m_X(other.m_X), m_Y(other.m_Y)
    {
        m_Cells = std::move(other.m_Cells);
    }

    Grid& operator=(Grid&& other)
    {
        m_X = other.m_X; m_Y = other.m_Y;
        m_Cells = std::move(other.m_Cells);
    }

    Cell& Get(int x, int y);
    void Set(int x, int y, const Cell& cell);

    inline int SizeX()
    {
        return m_X;
    }

    inline int SizeY()
    {
        return m_Y;
    }

    inline std::vector<Cell>::iterator begin()
    {
        return m_Cells.begin();
    }

    inline std::vector<Cell>::iterator end()
    {
        return m_Cells.end();
    }

private:
    int m_X, m_Y;
    std::vector<Cell> m_Cells;
};