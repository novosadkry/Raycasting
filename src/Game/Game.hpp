#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

class Game
{
public:
    Game(std::unique_ptr<sf::RenderWindow> window);

    void PollEvents();
    void Update();
    void Render();

    inline bool Game::IsRunning() const
    {
        return m_Window->isOpen();
    }

    inline void Game::Exit()
    {
        m_Window->close();
    }

    inline std::unique_ptr<sf::RenderWindow>& Game::GetWindow()
    {
        return m_Window;
    }

private:
    std::unique_ptr<sf::RenderWindow> m_Window;
};