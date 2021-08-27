#pragma once
#include <Core.hpp>

#include "Object.hpp"
#include "Player.hpp"
#include "Canvas.hpp"

class LevelView : public Object
{
public:
    LevelView(std::shared_ptr<Player> player, sf::Vector2u resolution, Canvas canvas)
        : m_Player(player), m_Canvas(canvas)
    {
        auto buffer = new sf::RenderTexture();

        if (!buffer->create(resolution.x, resolution.y))
            LOG("Unable to create RenderTexture!");

        m_Buffer = std::unique_ptr<sf::RenderTexture>(buffer);
    }

    void Render(float dt) override;
    void RenderView();

private:
    Canvas m_Canvas;
    std::shared_ptr<Player> m_Player;
    std::unique_ptr<sf::RenderTexture> m_Buffer;
};