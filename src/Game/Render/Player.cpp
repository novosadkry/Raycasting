#include "Player.hpp"

#include <Game/Game.hpp>
#include <Input/Input.hpp>
#include <Utils/Math.hpp>

void Player::SetDirection(float newDir)
{
    m_Direction = newDir - floor(newDir / 360.0f) * 360.0f;
}

void Player::Move(float amount)
{
    float x = cos(m_Direction / 180.0f * PI);
    float y = sin(m_Direction / 180.0f * PI);

    m_Position.x += x * amount;
    m_Position.y += y * amount;
}

void Player::Rotate(float amount)
{
    SetDirection(GetDirection() + amount);
}

static void CheckBounds(sf::Vector2f& pos, float radius, const sf::Vector2i& bounds)
{
    if (pos.x > bounds.x - radius)
        pos.x = bounds.x - radius;

    if (pos.y > bounds.y - radius)
        pos.y = bounds.y - radius;

    if (pos.x < radius)
        pos.x = radius;

    if (pos.y < radius)
        pos.y = radius;
}

static void HandleInput(float dt, Player& player)
{
    if (Input::GetKey(sf::Keyboard::W))
        player.Move(player.GetSpeed() * dt);

    if (Input::GetKey(sf::Keyboard::S))
        player.Move(-player.GetSpeed() * dt);

    if (Input::GetKey(sf::Keyboard::A))
        player.Rotate(-player.GetSpeed() * dt);

    if (Input::GetKey(sf::Keyboard::D))
        player.Rotate(player.GetSpeed() * dt);
}

void Player::Update(float dt)
{
    HandleInput(dt, *this);
    CheckBounds(m_Position, m_Radius, Game::Get().GetCurrentLevel().GetSize());
}