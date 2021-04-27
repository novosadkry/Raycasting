#include "Grid.hpp"

Cell& Grid::Get(int x, int y)
{
    int i = y * m_X + x;
    return m_Cells[i];
}

void Grid::Set(int x, int y, const Cell& cell)
{
    int i = y * m_X + x;
    m_Cells[i] = cell;
}

Cell& Grid::Get(sf::Vector2i pos)
{
    return Get(pos.x, pos.y);
}

void Grid::Set(sf::Vector2i pos, const Cell& cell)
{
    Set(pos.x, pos.y, cell);
}