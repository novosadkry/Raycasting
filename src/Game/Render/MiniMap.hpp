#pragma once
#include "Object.hpp"
#include "Player.hpp"

class MiniMap : public Object
{
public:
    MiniMap(std::shared_ptr<Player> player, sf::Vector2i size)
        : m_Size(size), m_Player(player) { }

    void Render(float dt) override;

private:
    sf::Vector2i m_Size;
    std::shared_ptr<Player> m_Player;
};