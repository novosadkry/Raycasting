#pragma once
#include <Core.hpp>

#include <Game/Grid/Grid.hpp>
#include <Game/Render/Light.hpp>
#include <Game/Hierarchy/Object.hpp>
#include <Game/Hierarchy/Hierarchy.hpp>

class Level : Serializable<Level>
{
public:
    SERIALIZE_BASE(Level)

    static const Level Empty;
    static Unique<Level> From(const char* path);
    static void Save(Level& level, const char* path);

public:
    Level(sf::Vector2i size, Grid grid)
        : m_Size(size), m_Grid(std::move(grid)) { }

    Level(int x, int y, Grid grid)
        : m_Size({x, y}), m_Grid(std::move(grid)) { }

    inline Grid& GetGrid()
    {
        return m_Grid;
    }

    inline void SetGrid(Grid grid)
    {
        m_Grid = std::move(grid);
    }

    inline sf::Vector2i GetSize()
    {
        return m_Size;
    }

    inline Hierarchy& GetHierarchy()
    {
        return m_Hierarchy;
    }

    inline std::vector<Light>& GetLights()
    {
        return m_Lights;
    }

    sf::Vector2i GetGridCellFromPos(sf::Vector2f pos);

    void Update(float dt);
    void Render(float dt);

    void OnLoad();
    void OnUnload();

private:
    Grid m_Grid;
    sf::Vector2i m_Size;
    Hierarchy m_Hierarchy;
    std::vector<Light> m_Lights;
};