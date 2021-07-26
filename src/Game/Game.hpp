#pragma once

#include <Utils/Debug.hpp>

#include "Level.hpp"
#include "Render/Player.hpp"

class Game
{
public:
    inline static Game& Get()
    {
        return *s_Instance;
    }

    template<typename ...Args>
    static Game& Init(Args&& ...args)
    {
        ASSERT(!s_Instance);

        auto rw = std::make_unique<sf::RenderWindow>(std::forward<Args>(args)...);
        rw->setVerticalSyncEnabled(true);

        s_Instance = new Game(std::move(rw));
        return *s_Instance;
    }

private:
    static Game* s_Instance;

public:
    Game(std::unique_ptr<sf::RenderWindow> window);

    void Tick();
    void PollEvents();
    void Update(float dt);
    void Render(float dt);

    inline bool IsRunning() const
    {
        return m_Window->isOpen();
    }

    inline void Exit()
    {
        m_Window->close();
    }

    inline sf::RenderWindow& GetWindow()
    {
        return *m_Window;
    }

    inline Level& GetCurrentLevel()
    {
        return *m_CurrentLevel;
    }

    void LoadLevel(Level level);

private:
    std::unique_ptr<sf::RenderWindow> m_Window;
    std::unique_ptr<Level> m_CurrentLevel;
};