#include "Level.hpp"

#include <Game/Game.hpp>
#include <Game/ECS/Entity.hpp>

#include <Game/Render/Layers/MiniMap.hpp>
#include <Game/Render/Layers/LevelView.hpp>
#include <Game/Render/Layers/DebugMenu.hpp>

#include <Game/ECS/Components/Player.hpp>
#include <Game/ECS/Components/Collider.hpp>
#include <Game/ECS/Components/Transform.hpp>
#include <Game/ECS/Components/Light.hpp>
#include <Game/ECS/Systems/Collision.hpp>
#include <Game/ECS/Systems/Controller.hpp>

#include <Utils/Serialize.impl.hpp>

Unique<Level> Level::Empty()
{
    return MakeUnique<Level>(0, 0, Grid({0, 0}, {}));
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
    float cellWidth = (float) m_Size.x / m_Grid.GetSize().x;
    float cellHeight = (float) m_Size.y / m_Grid.GetSize().y;

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
    auto e = m_Hierarchy.CreateEntity();
    e.Add<ECS::Components::Player>(100.0f);
    e.Add<ECS::Components::Transform>(sf::Vector2f{100, 100}, 0);
    e.Add<ECS::Components::Collider>(16, 10);
    e.Add<ECS::Components::Light>(sf::Color::White, 1);

    m_Systems.Add<ECS::Systems::Controller>();
    m_Systems.Add<ECS::Systems::Collision>();

    auto& layers = Game::Get().GetLayers();
    layers.Emplace<LevelView, true>(sf::Vector2u(300, 200), Canvas::From(75.0f * Math::Deg2Rad));
    layers.Emplace<MiniMap,   true>(sf::Vector2i(200, 200));
    layers.Emplace<DebugMenu, true>(); // ? Shouldn't this be somewhere else?
}

void Level::OnUnload()
{
    auto& layers = Game::Get().GetLayers();
    layers.Drop<LevelView, MiniMap>();
}
