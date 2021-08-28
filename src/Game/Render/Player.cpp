#include "Player.hpp"

#include <Game/Game.hpp>
#include <Input/Input.hpp>
#include <Utils/Math.hpp>
#include <Game/Ray.hpp>

SERIALIZE_HIERARCHY_REGISTER(Player)

void Player::SetRotation(float newDir)
{
    m_Rotation = newDir - floor(newDir / Math::PI_2) * Math::PI_2;
}

void Player::Move(float amount)
{
    sf::Vector2f dir = Math::Angle2Vector(m_Rotation);
    m_Position += dir * amount;
}

void Player::Strafe(float amount)
{
    sf::Vector2f dir = Math::Angle2Vector(m_Rotation + Math::PI / 2);
    m_Position += dir * amount;
}

void Player::Rotate(float amount)
{
    SetRotation(GetRotation() + amount);
}

void Player::CheckBounds()
{
    auto bounds = Game::Get().GetCurrentLevel().GetSize();

    if (m_Position.x > bounds.x - m_Radius)
        m_Position.x = bounds.x - m_Radius;

    if (m_Position.y > bounds.y - m_Radius)
        m_Position.y = bounds.y - m_Radius;

    if (m_Position.x < m_Radius)
        m_Position.x = m_Radius;

    if (m_Position.y < m_Radius)
        m_Position.y = m_Radius;
}

void Player::CheckCollision(int sides)
{
    auto& level = Game::Get().GetCurrentLevel();

    for (int i = 0; i < sides; i++)
    {
        float angle = i * (2.0f / sides) * Math::PI;

        Ray hit;
        if (Ray::Cast(level, m_Position, angle, hit))
        {
            if (hit.distance < m_Radius)
                m_Position -= hit.dir * (m_Radius - hit.distance);
        }
    }
}

void Player::HandleInput(float dt)
{
    if (Input::GetKey(Keyboard::W))
        Move(m_Speed * dt);

    if (Input::GetKey(Keyboard::S))
        Move(-m_Speed * dt);

    if (Input::GetKey(Keyboard::A))
    {
        Input::GetKey(Keyboard::LShift)
            ? Strafe(-m_Speed * dt)
            : Rotate(-m_Speed * dt * Math::Deg2Rad);
    }

    if (Input::GetKey(Keyboard::D))
    {
        Input::GetKey(Keyboard::LShift)
            ? Strafe(m_Speed * dt)
            : Rotate(m_Speed * dt * Math::Deg2Rad);
    }
}

void Player::Update(float dt)
{
    HandleInput(dt);
    CheckBounds();
    CheckCollision(16);
}

void Player::Serialize(std::ostream &stream) const
{

}

Player Player::Deserialize(std::istream &stream)
{
    throw std::logic_error("Function not yet implemented");
}