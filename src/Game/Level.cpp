#include "Level.hpp"
#include <Game/Render/MiniMap.hpp>
#include <Game/Render/LevelView.hpp>

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

    m_Objects.push_back(player);
    m_Objects.push_back(std::make_shared<MiniMap>(player));
    m_Objects.push_back(std::make_shared<LevelView>(player, 75.0f));
}