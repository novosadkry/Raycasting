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

    bool IsRunning();
    void Exit();

private:
    std::unique_ptr<sf::RenderWindow> m_Window;
};