#include "Level.hpp"

#include <Game/Game.hpp>
#include <Game/ECS/ECS.hpp>

#include <Utils/Serialize.impl.hpp>

Unique<Level> Level::Empty()
{
    return Empty({0, 0}, {0, 0});
}

Unique<Level> Level::Empty(sf::Vector2i ls, sf::Vector2i gs)
{
    return MakeUnique<Level>(ls, MakeUnique<Grid>(gs));
}

Unique<Level> Level::From(std::fs::path path)
{
    auto level = Level::Empty();

    try
    {
        std::ifstream file(path, std::ios_base::binary);
        if (!file) throw std::exception();

        cereal::BinaryInputArchive archive(file);
        archive(*level);
    }
    catch (std::exception& e)
	{
        LOG("Failed to load level data from " << path)
	}

    path.replace_extension(".res");
    auto res = ResourceMap::From(path);
    level->m_Resources = std::move(res);

    return level;
}

void Level::Save(Level& level, std::fs::path path)
{
    try
    {
        std::ofstream file(path, std::ios_base::binary);
        if (!file) throw std::exception();

        cereal::BinaryOutputArchive archive(file);
        archive(level);
    }
    catch (std::exception& e)
	{
        LOG("Failed to save level data to " << path)
	}

    path.replace_extension(".res");
    ResourceMap::Save(*level.m_Resources, path);
}

sf::Vector2i Level::GetGridCellFromPos(sf::Vector2f pos)
{
    sf::Vector2f cellSize = m_Grid->GetCellSize(*this);

    int cellX = (int) floor(pos.x / cellSize.x);
    int cellY = (int) floor(pos.y / cellSize.y);

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
}

void Level::OnUnload()
{

}
