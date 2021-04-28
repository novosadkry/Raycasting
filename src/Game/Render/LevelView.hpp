#pragma once

#include "Object.hpp"
#include "Player.hpp"
#include <memory>

class LevelView : public Object
{
public:
    LevelView(std::shared_ptr<Player> player, float fov)
        : m_Player(player), m_FOV(fov) {}

    static float Trace(Level& level, Player& player, float angle);

    void Render(float dt) override;

private:
    float m_FOV;
    std::shared_ptr<Player> m_Player;
};