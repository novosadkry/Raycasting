#pragma once
#include <Core.hpp>

#include <Game/ECS/Entity.hpp>
#include <Game/Render/Layer.hpp>
#include <Game/Render/Canvas.hpp>

class LevelView : public Layer
{
public:
    LevelView() = default;

    void Init() override;
    void Render(float dt) override;
    void RenderView();

private:
    Unique<sf::RenderTexture> m_Buffer;
};