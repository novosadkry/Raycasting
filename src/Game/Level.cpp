#include "Level.hpp"

#include <Game/Render/Layers/MiniMap.hpp>
#include <Game/Render/Layers/LevelView.hpp>

const Level Level::Empty = Level(0, 0, Grid({0, 0}, {}));

Unique<Level> Level::From(const char* path)
{
    std::ifstream file(path, std::ios_base::binary);
    return ::Deserialize<Level>(file);
}

void Level::Save(Level& level, const char* path)
{
    std::ofstream file(path, std::ios_base::binary);
    ::Serialize<Level>(level, file);
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
    for (auto&& [type, obj] : m_Hierarchy)
        obj->Update(dt);
}

void Level::OnLoad()
{
    auto player = MakeShared<Player>(10.0f, 100.0f);
    player->SetPosition({100, 100});
    player->SetRotation(0);

    m_Lights.push_back(Light({355, 400}, 1, sf::Color::Red));
    m_Lights.push_back(Light({150, 420}, 1, sf::Color::Green));
    m_Lights.push_back(Light({150, 120}, 1, sf::Color::Blue));
    m_Lights.push_back(Light({350, 150}, 1, sf::Color::Yellow));

    m_Hierarchy.AddObject(player);

    for (auto&& [type, obj] : m_Hierarchy)
        obj->Init();

    // ! Everything above will be removed when deserialization gets implemented

    auto& layers = Game::Get().GetLayers();
    layers.Emplace<LevelView, true>(sf::Vector2u(300, 200), Canvas::From(75.0f * Math::Deg2Rad));
    layers.Emplace<MiniMap,   true>(sf::Vector2i(200, 200));
}

void Level::OnUnload()
{
    auto& layers = Game::Get().GetLayers();
    layers.Drop<LevelView, MiniMap>();
}

void Level::Serialize(std::ostream &stream) const
{
    ::Serialize<sf::Vector2i>(m_Size, stream);
    ::Serialize<Grid>(m_Grid, stream);
    ::Serialize<Hierarchy>(m_Hierarchy, stream);
}

Unique<Level> Level::Deserialize(std::istream &stream)
{
    sf::Vector2i levelSize = ::Deserialize<sf::Vector2i>(stream);
    Unique<Grid> grid = ::Deserialize<Grid>(stream);

    return MakeUnique<Level>(levelSize, *grid);
}