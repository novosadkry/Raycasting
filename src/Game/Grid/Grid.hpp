#pragma once
#include "Cell.hpp"

#include <SFML/System/Vector2.hpp>
#include <vector>

class Grid
{
public:
    Grid(sf::Vector2i size)
        : m_Size(size), m_Cells(size.x * size.y) { }

    Grid(sf::Vector2i size, Cell* array)
        : m_Size(size), m_Cells(array, array + size.x * size.y) { }

    Grid(sf::Vector2i size, std::vector<Cell> array)
        : m_Size(size), m_Cells(std::move(array)) { }

    Grid(Grid&& other)
        : m_Size(other.m_Size), m_Cells(std::move(other.m_Cells)) { }

    Grid& operator=(Grid&& other)
    {
        m_Size = other.m_Size;
        m_Cells = std::move(other.m_Cells);
    }

    Cell Get(int x, int y);
    Cell Get(sf::Vector2i pos);

    void Set(int x, int y, const Cell& cell);
    void Set(sf::Vector2i pos, const Cell& cell);

    inline sf::Vector2i GetSize()
    {
        return m_Size;
    }

    inline std::vector<Cell>::iterator begin()
    {
        return m_Cells.begin();
    }

    inline std::vector<Cell>::iterator end()
    {
        return m_Cells.end();
    }

    inline std::vector<Cell>& GetCells()
    {
        return m_Cells;
    }

private:
    sf::Vector2i m_Size;
    std::vector<Cell> m_Cells;
};