#include "Game.hpp"

#include <Game/ECS/ECS.hpp>

#include <Game/Render/Layers/DebugMenu.hpp>
#include <Game/Render/Layers/MiniMap.hpp>
#include <Game/Render/Layers/LevelView.hpp>

#include <Input/Input.hpp>

static sf::Clock deltaClock;

Game* Game::s_Instance = nullptr;

Game::Game(Unique<sf::RenderWindow> window)
    : m_Window(std::move(window))
{
    ImGui::SFML::Init(*m_Window);
    ECS::Init(ECS::AllComponents{});

    m_Layers.Emplace<DebugMenu, true>();
    m_Layers.Emplace<LevelView, true>();
    m_Layers.Emplace<MiniMap,   true>(sf::Vector2i(200, 200));
}

void Game::PollEvents()
{
    Event event;
    while (GetWindow().pollEvent(event))
    {
        ImGui::SFML::ProcessEvent(event);
        Input::HandleEvent(event);
    }
}

void Game::LoadLevel(Unique<Level> level)
{
    if (m_CurrentLevel)
        UnloadLevel();

    m_CurrentLevel = std::move(level);
    m_CurrentLevel->OnLoad();
}

Unique<Level> Game::UnloadLevel()
{
    if (!m_CurrentLevel)
        return nullptr;

    m_CurrentLevel->OnUnload();
    return std::move(m_CurrentLevel);
}

void Game::Tick()
{
    sf::Time dt = deltaClock.restart();

    PollEvents();
    if (IsRunning())
    {
        Update(dt);
        Render(dt);
    }
}

void Game::Update(sf::Time dt)
{
    ImGui::SFML::Update(*m_Window, dt);
    m_CurrentLevel->Update(dt.asSeconds());
}

void Game::Render(sf::Time dt)
{
    m_Window->clear(sf::Color::Black);

    for (auto&& layer : m_Layers)
        layer->Render(dt.asSeconds());

    ImGui::SFML::Render(*m_Window);

    m_Window->display();
}