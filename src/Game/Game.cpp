#include "Game.hpp"

#include <Input/Input.hpp>

static sf::Clock deltaClock;

Game* Game::s_Instance = nullptr;

Game::Game(Unique<sf::RenderWindow> window)
    : m_Window(std::move(window)) { }

void Game::PollEvents()
{
    Event event;
    while (GetWindow().pollEvent(event))
        Input::HandleEvent(event);
}

void Game::LoadLevel(Unique<Level> level)
{
    if (m_CurrentLevel)
        m_CurrentLevel->OnUnload();

    m_CurrentLevel = std::move(level);
    m_CurrentLevel->OnLoad();
}

void Game::Tick()
{
    float dt = deltaClock.restart().asSeconds();

    PollEvents();
    Update(dt);
    Render(dt);
}

void Game::Update(float dt)
{
    m_CurrentLevel->Update(dt);
}

void Game::Render(float dt)
{
    m_Window->clear(sf::Color::Black);

    for (auto&& layer : m_Layers)
        layer->Render(dt);

    m_Window->display();
}