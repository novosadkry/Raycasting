#pragma once

#include "Object.hpp"
#include "Player.hpp"
#include "Canvas.hpp"

#include <Utils/Math.hpp>
#include <memory>

class LevelView : public Object
{
public:
    LevelView(std::shared_ptr<Player> player, const Canvas& canvas)
        : m_Player(player), m_Canvas(canvas) { }

    void Render(float dt) override;

private:
    Canvas m_Canvas;
    std::shared_ptr<Player> m_Player;
};