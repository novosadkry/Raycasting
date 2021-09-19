#pragma once
#include <Core.hpp>

#include <Game/Level.hpp>
#include <Game/Render/Layer.hpp>

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
    void Tick();
    void PollEvents();
    void Update(sf::Time dt);
    void Render(sf::Time dt);

    inline bool IsRunning() const
    {
        return m_Window->isOpen();
    }

    inline void Exit()
    {
        m_Window->close();
        ImGui::SFML::Shutdown();
    }

    inline LayerStack& GetLayers()
    {
        return m_Layers;
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
    Unique<Level> UnloadLevel();

private:
    Game(Unique<sf::RenderWindow> window);
    Game(const Game&) = delete;

    LayerStack m_Layers;

    Unique<sf::RenderWindow> m_Window;
    Unique<Level> m_CurrentLevel;
};