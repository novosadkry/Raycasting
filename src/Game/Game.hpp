#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

#include "Level/Level.hpp"

class Game
{
public:
    Game(std::unique_ptr<sf::RenderWindow> window);

    void PollEvents();
    void Update();
    void Render();

    inline bool IsRunning() const
    {
        return m_Window->isOpen();
    }

    inline void Exit()
    {
        m_Window->close();
    }

    inline std::unique_ptr<sf::RenderWindow>& GetWindow()
    {
        return m_Window;
    }

    inline void LoadLevel(const std::shared_ptr<Level>& level)
    {
        m_CurrentLevel = level;
    }

private:
    std::unique_ptr<sf::RenderWindow> m_Window;
    std::shared_ptr<Level> m_CurrentLevel;
};