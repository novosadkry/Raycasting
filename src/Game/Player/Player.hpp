#pragma once

#include <SFML/System/Vector2.hpp>

class Player
{
public:
    Player() {}

    inline sf::Vector2f GetPosition()
    {
        return m_Position;
    }

    inline void SetPosition(sf::Vector2f newPos)
    {
        m_Position = newPos;
    }

    inline float GetDirection()
    {
        return m_Direction;
    }

    void SetDirection(float newDir);

    void Move(float amount);

private:
    sf::Vector2f m_Position;
    float m_Direction;
};