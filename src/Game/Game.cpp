#include "Game.hpp"

#include <Input/Input.hpp>

static sf::Clock deltaClock;

Game Game::s_Instance = 0;

Game::Game(std::unique_ptr<sf::RenderWindow> window)
    : m_Window(std::move(window)) { }

void Game::PollEvents()
{
    sf::Event event;
    while (GetWindow().pollEvent(event))
        Input::HandleEvent(event);
}

void Game::LoadLevel(std::shared_ptr<Level> level)
{
    if (m_CurrentLevel)
        m_CurrentLevel->OnUnload();

    m_CurrentLevel = level;
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
    m_CurrentLevel->Render(dt);
    m_Window->display();
}