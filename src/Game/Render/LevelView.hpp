#pragma once
#include <Core.hpp>

#include <Game/Render/Canvas.hpp>
#include <Game/Hierarchy/Objects/Player.hpp>

class LevelView : public Object
{
public:
    LevelView(sf::Vector2u resolution, Canvas canvas)
        : m_Canvas(canvas)
    {
        auto buffer = new sf::RenderTexture();

        if (!buffer->create(resolution.x, resolution.y))
            LOG("Unable to create RenderTexture!");

        m_Buffer = Unique<sf::RenderTexture>(buffer);
    }

    void Init() override;
    void Render(float dt) override;
    void RenderView();

private:
    Canvas m_Canvas;
    Unique<sf::RenderTexture> m_Buffer;

    Shared<Player> m_Player;
};