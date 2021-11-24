#pragma once
#include <Core.hpp>

#include <Game/Resource.hpp>
#include <Game/Grid/Grid.hpp>
#include <Game/ECS/System.hpp>
#include <Game/ECS/Hierarchy.hpp>

class Level
{
public:
    static Unique<Level> Empty();
    static Unique<Level> Empty(sf::Vector2i ls, sf::Vector2i gs);

    static Unique<Level> From(std::fs::path path);
    static void Save(Level& level, std::fs::path path);

public:
    Level(const Level&) = delete;

    Level(sf::Vector2i size, Unique<Grid> grid) :
        m_Size(size),
        m_Grid(std::move(grid)),
        m_Hierarchy(),
        m_Systems(&m_Hierarchy.GetRegistry()),
        m_Resources(MakeUnique<ResourceMap>())
    { }

    Level(int x, int y, Unique<Grid> grid) :
        m_Size({x, y}),
        m_Grid(std::move(grid)),
        m_Hierarchy(),
        m_Systems(&m_Hierarchy.GetRegistry()),
        m_Resources(MakeUnique<ResourceMap>())
    { }

    inline Grid& GetGrid()
    {
        return *m_Grid;
    }

    inline void SetGrid(Unique<Grid> grid)
    {
        m_Grid = std::move(grid);
    }

    inline sf::Vector2i GetSize()
    {
        return m_Size;
    }

    inline const std::string& GetName()
    {
        return m_Name;
    }

    inline void SetName(const std::string& name)
    {
        m_Name = name;
    }

    inline ECS::Hierarchy& GetHierarchy()
    {
        return m_Hierarchy;
    }

    inline ECS::SystemGroup& GetSystems()
    {
        return m_Systems;
    }

    inline ResourceMap& GetResources()
    {
        return *m_Resources;
    }

    sf::Vector2i GetGridCellFromPos(sf::Vector2f pos);

    void OnLoad();
    void OnUnload();

    void Update(float dt);

private:
    SERIALIZE_PRIVATE(Level)

    Unique<Grid> m_Grid;
    sf::Vector2i m_Size;

    ECS::Hierarchy m_Hierarchy;
    ECS::SystemGroup m_Systems;

    std::string m_Name = "Empty";

    Unique<ResourceMap> m_Resources;
};