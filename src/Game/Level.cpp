#include "Level.hpp"

#include <Utils/Math.hpp>
#include <Game/Render/MiniMap.hpp>
#include <Game/Render/LevelView.hpp>

sf::Vector2i Level::GetGridCellFromPos(sf::Vector2f pos)
{
    float cellWidth = (float) m_Size.x / m_Grid.SizeX();
    float cellHeight = (float) m_Size.y / m_Grid.SizeY();

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

    m_Objects.push_back(player);
    m_Objects.push_back(std::make_shared<LevelView>(player, Canvas::From(75.0f * Math::Deg2Rad)));
    m_Objects.push_back(std::make_shared<MiniMap>(player, sf::Vector2i(200, 200)));
}