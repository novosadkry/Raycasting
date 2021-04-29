#pragma once

#include <Game/Game.hpp>
#include <SFML/System/Vector2.hpp>
#include "Object.hpp"

class Player : public Object
{
public:
    Player(float radius, float speed)
        : m_Radius(radius), m_Speed(speed) { }

    inline float GetRadius()
    {
        return m_Radius;
    }

    inline sf::Vector2f GetPosition()
    {
        return m_Position;
    }

    inline void SetPosition(sf::Vector2f newPos)
    {
        m_Position = newPos;
    }

    inline float GetRotation()
    {
        return m_Rotation;
    }

    void SetRotation(float newDir);

    inline float GetSpeed()
    {
        return m_Speed;
    }

    void Move(float amount);
    void Rotate(float amount);

    void Update(float dt) override;

private:
    sf::Vector2f m_Position;
    float m_Rotation;
    float m_Radius;
    float m_Speed;
};