#pragma once
#include <Core.hpp>

#include <Game/Grid/Grid.hpp>
#include <Game/ECS/System.hpp>
#include <Game/ECS/Hierarchy.hpp>

class Level : Serializable<Level>
{
public:
    SERIALIZE_BASE(Level)

    static Unique<Level> Empty();
    static Unique<Level> From(const char* path);
    static void Save(Level& level, const char* path);

public:
    Level(const Level&) = delete;

    Level(sf::Vector2i size, Grid grid) :
        m_Size(size),
        m_Grid(std::move(grid)),
        m_Hierarchy(),
        m_Systems(&m_Hierarchy.GetRegistry())
    { }

    Level(int x, int y, Grid grid) :
        m_Size({x, y}),
        m_Grid(std::move(grid)),
        m_Hierarchy(),
        m_Systems(&m_Hierarchy.GetRegistry())
    { }

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

    inline ECS::Hierarchy& GetHierarchy()
    {
        return m_Hierarchy;
    }

    inline ECS::SystemGroup& GetSystems()
    {
        return m_Systems;
    }

    sf::Vector2i GetGridCellFromPos(sf::Vector2f pos);

    void OnLoad();
    void OnUnload();

    void Update(float dt);

private:
    Grid m_Grid;
    sf::Vector2i m_Size;

    ECS::Hierarchy m_Hierarchy;
    ECS::SystemGroup m_Systems;
};