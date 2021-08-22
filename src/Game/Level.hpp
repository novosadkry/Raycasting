#pragma once
#include <Rpch.hpp>

#include <Utils/Serialize.hpp>
#include <Game/Grid/Grid.hpp>
#include <Game/Render/Light.hpp>
#include <Game/Render/Object.hpp>

class Level : Serializable<Level>
{
public:
    static Level Empty;
    static Level From(const char* path);
    static void Save(Level& level, const char* path);
    static std::unique_ptr<Level> Deserialize(std::istream& stream);

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

    inline std::vector<Light>& GetLights()
    {
        return m_Lights;
    }

    sf::Vector2i GetGridCellFromPos(sf::Vector2f pos);

    void Update(float dt);
    void Render(float dt);

    void OnLoad();
    void OnUnload();

    void Serialize(std::ostream& stream) override;

private:
    Grid m_Grid;
    sf::Vector2i m_Size;
    std::vector<Light> m_Lights;
    std::vector<std::shared_ptr<Object>> m_Objects;
};