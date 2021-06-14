#include "Level.hpp"

#include <Utils/Math.hpp>
#include <Game/Render/MiniMap.hpp>
#include <Game/Render/LevelView.hpp>

#include <fstream>

Level Level::Empty = Level(0, 0, Grid({0, 0}, {}));

Level Level::From(const char* path)
{
    std::ifstream file(path, std::ios_base::binary);

    sf::Vector2i levelSize;
    file.read(reinterpret_cast<char*>(&levelSize), sizeof(levelSize));

    sf::Vector2i gridSize;
    file.read(reinterpret_cast<char*>(&gridSize), sizeof(gridSize));

    Cell* cells = new Cell[gridSize.x * gridSize.y];
    file.read(reinterpret_cast<char*>(cells), sizeof(Cell) * gridSize.x * gridSize.y);

    Grid grid(gridSize, cells);
    delete cells;

    return Level(levelSize, std::move(grid));
}

void Level::Save(Level& level, const char* path)
{
    std::ofstream file(path, std::ios_base::binary);

    Grid& grid = level.GetGrid();
    sf::Vector2i gridSize = grid.GetSize();

    file.write(reinterpret_cast<char*>(&level.m_Size), sizeof(sf::Vector2i));
    file.write(reinterpret_cast<char*>(&gridSize), sizeof(sf::Vector2i));
    file.write(reinterpret_cast<char*>(grid.GetCells().data()), sizeof(Cell) * gridSize.x * gridSize.y);
}

sf::Vector2i Level::GetGridCellFromPos(sf::Vector2f pos)
{
    float cellWidth = (float) m_Size.x / m_Grid.GetSize().x;
    float cellHeight = (float) m_Size.y / m_Grid.GetSize().y;

    int cellX = (int) floor(pos.x / cellWidth);
    int cellY = (int) floor(pos.y / cellHeight);

    return { cellX, cellY };
}

void Level::Render(float dt)
{
    for (auto& obj : m_Objects)
        obj->Render(dt);
}

void Level::Update(float dt)
{
    for (auto& obj : m_Objects)
        obj->Update(dt);
}

void Level::OnLoad()
{
    auto player = std::make_shared<Player>(10.0f, 100.0f);
    player->SetPosition({100, 100});
    player->SetRotation(0);

    m_Objects.push_back(player);
    m_Objects.push_back(std::make_shared<LevelView>(player, sf::Vector2u(300, 200), Canvas::From(75.0f * Math::Deg2Rad)));
    m_Objects.push_back(std::make_shared<MiniMap>(player, sf::Vector2i(200, 200)));
}

void Level::OnUnload()
{
    m_Objects.clear();
}