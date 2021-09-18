#include "Grid.hpp"

Cell Grid::Get(int x, int y)
{
    int i = y * m_Size.x + x;

    if (x < 0 || x >= m_Size.x || y < 0 || y >= m_Size.y)
        return Empty;

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

void Grid::Serialize(std::ostream& stream) const
{
    ::Serialize<sf::Vector2i>(m_Size, stream);
    ::Serialize<Cell>(m_Cells.data(), m_Cells.size(), stream);
}

Unique<Grid> Grid::Deserialize(std::istream& stream)
{
    sf::Vector2i gridSize = ::Deserialize<sf::Vector2i>(stream);
    Cell* cells = ::Deserialize<Cell>(gridSize.x * gridSize.y, stream);

    auto grid = MakeUnique<Grid>(gridSize, cells);
    delete cells;

    return grid;
}