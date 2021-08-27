#pragma once
#include <Core.hpp>

#include <Game/Game.hpp>
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
    void Strafe(float amount);
    void Rotate(float amount);

    void Update(float dt) override;

private:
    void HandleInput(float dt);

    void CheckBounds();
    void CheckCollision(int sides);

    sf::Vector2f m_Position;
    float m_Rotation;
    float m_Radius;
    float m_Speed;
};