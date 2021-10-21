#include "Level.hpp"

#include <Game/Game.hpp>
#include <Game/ECS/ECS.hpp>

#include <Game/Render/Layers/MiniMap.hpp>
#include <Game/Render/Layers/LevelView.hpp>

#include <Utils/Serialize.impl.hpp>

Unique<Level> Level::Empty()
{
    return Empty({0, 0}, {0, 0});
}

Unique<Level> Level::Empty(sf::Vector2i ls, sf::Vector2i gs)
{
    return MakeUnique<Level>(ls, MakeUnique<Grid>(gs));
}

Unique<Level> Level::From(const char* path)
{
    std::ifstream file(path, std::ios_base::binary);
    cereal::BinaryInputArchive archive(file);

    auto level = Level::Empty();
    archive(*level);

    return level;
}

void Level::Save(Level& level, const char* path)
{
    std::ofstream file(path, std::ios_base::binary);
    cereal::BinaryOutputArchive archive(file);

    archive(level);
}

sf::Vector2i Level::GetGridCellFromPos(sf::Vector2f pos)
{
    float cellWidth =  (float) m_Size.x / m_Grid->GetSize().x;
    float cellHeight = (float) m_Size.y / m_Grid->GetSize().y;

    int cellX = (int) floor(pos.x / cellWidth);
    int cellY = (int) floor(pos.y / cellHeight);

    return { cellX, cellY };
}

void Level::Update(float dt)
{
    m_Systems.Update(dt);
}

void Level::OnLoad()
{
    m_Systems.Add<ECS::Systems::Controller>();
    m_Systems.Add<ECS::Systems::Collision>();

    auto& layers = Game::Get().GetLayers();
    layers.Emplace<LevelView, true>(sf::Vector2u(300, 200), Canvas::From(75.0f * Math::Deg2Rad));
    layers.Emplace<MiniMap,   true>(sf::Vector2i(200, 200));
}

void Level::OnUnload()
{
    auto& layers = Game::Get().GetLayers();
    layers.Drop<LevelView, MiniMap>();
}
