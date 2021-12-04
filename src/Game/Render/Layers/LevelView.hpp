#pragma once
#include <Core.hpp>

#include <Game/ECS/Entity.hpp>
#include <Game/Render/Layer.hpp>
#include <Game/Render/Canvas.hpp>

class LevelView : public Layer
{
public:
    LevelView(Canvas canvas)
        : m_Canvas(canvas)
    {
        auto buffer = new sf::RenderTexture();

        if (!buffer->create(canvas.size.x, canvas.size.y))
            LOG("Unable to create RenderTexture!");

        m_Buffer = Unique<sf::RenderTexture>(buffer);
    }

    void Init() override;
    void Render(float dt) override;
    void RenderView();

private:
    Canvas m_Canvas;
    Unique<sf::RenderTexture> m_Buffer;
};