#pragma once
#include <Core.hpp>

#include "Object.hpp"
#include "Player.hpp"
#include "Canvas.hpp"

class LevelView : public Object
{
public:
    LevelView(Shared<Player> player, sf::Vector2u resolution, Canvas canvas)
        : m_Player(player), m_Canvas(canvas)
    {
        auto buffer = new sf::RenderTexture();

        if (!buffer->create(resolution.x, resolution.y))
            LOG("Unable to create RenderTexture!");

        m_Buffer = Unique<sf::RenderTexture>(buffer);
    }

    void Render(float dt) override;
    void RenderView();

private:
    Canvas m_Canvas;
    Shared<Player> m_Player;
    Unique<sf::RenderTexture> m_Buffer;
};