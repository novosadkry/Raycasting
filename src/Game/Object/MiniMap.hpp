#pragma once
#include "Object.hpp"
#include "Player.hpp"

class MiniMap : public Object
{
public:
    MiniMap(std::shared_ptr<Player> player)
        : m_Player(player) { }

    void Render(float dt) override;

private:
    std::shared_ptr<Player> m_Player;
};