#include "Grid.hpp"
#include <Game/Level.hpp>

sf::Vector2f Grid::GetCellSize(Level& level)
{
    return sf::Vector2f {
        (float) level.GetSize().x / level.GetGrid().GetSize().x,
        (float) level.GetSize().y / level.GetGrid().GetSize().y
    };
}

Cell Grid::Get(int x, int y)
{
    int i = y * m_Size.x + x;

    if (x < 0 || x >= m_Size.x || y < 0 || y >= m_Size.y)
        return Cell::Empty;

    return m_Cells[i];
}

void Grid::Set(int x, int y, const Cell& cell)
{
    int i = y * m_Size.x + x;
    m_Cells[i] = cell;
}

Cell Grid::Get(sf::Vector2i pos)
{
    return Get(pos.x, pos.y);
}

void Grid::Set(sf::Vector2i pos, const Cell& cell)
{
    Set(pos.x, pos.y, cell);
}
