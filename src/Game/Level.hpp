#pragma once

#include <SFML/System/Vector2.hpp>

#include <Game/Grid/Grid.hpp>
#include <Game/Render/Object.hpp>

#include <vector>
#include <memory>

class Level
{
public:
    Level(sf::Vector2i size, std::unique_ptr<Grid> grid)
        : m_Size(size), m_Grid(std::move(grid)) { }

    Level(int x, int y, std::unique_ptr<Grid> grid)
        : m_Size({x, y}), m_Grid(std::move(grid)) { }

    Level(int x, int y)
        : m_Size({x, y}) { }

    inline Grid& GetGrid()
    {
        return *m_Grid;
    }

    inline void SetGrid(std::unique_ptr<Grid> grid)
    {
        m_Grid = std::move(grid);
    }

    inline sf::Vector2i GetSize()
    {
        return m_Size;
    }

    void Update(float dt);
    void Render(float dt);

    void OnLoad();

private:
    sf::Vector2i m_Size;
    std::unique_ptr<Grid> m_Grid;
    std::vector<std::shared_ptr<Object>> m_Objects;
};