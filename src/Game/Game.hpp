#pragma once

#include <Utils/Debug.hpp>
#include <Game/Level.hpp>

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

        auto rw = MakeUnique<sf::RenderWindow>(std::forward<Args>(args)...);
        rw->setVerticalSyncEnabled(true);

        s_Instance = new Game(std::move(rw));
        return *s_Instance;
    }

private:
    static Game* s_Instance;

public:
    Game(Unique<sf::RenderWindow> window);

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

    void LoadLevel(Unique<Level> level);

private:
    Unique<sf::RenderWindow> m_Window;
    Unique<Level> m_CurrentLevel;
};