#include "Player.hpp"

#include <Game/Game.hpp>
#include <Input/Input.hpp>
#include <Utils/Math.hpp>
#include <Game/Ray.hpp>

void Player::SetRotation(float newDir)
{
    m_Rotation = newDir - floor(newDir / Math::PI_2) * Math::PI_2;
}

void Player::Move(float amount)
{
    sf::Vector2f dir = Math::Angle2Vector(m_Rotation);
    m_Position += dir * amount;
}

void Player::Rotate(float amount)
{
    SetRotation(GetRotation() + amount);
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

static void CheckCollision(Level& level, Player& player)
{
    sf::Vector2f pos = player.GetPosition();

    for (int i = 0; i < 8; i++)
    {
        float angle = i * 0.25f * Math::PI;

        Ray hit;
        if (Ray::Cast(level, pos, angle, hit))
        {
            if (hit.distance < player.GetRadius())
                player.SetPosition(pos - hit.dir * (player.GetRadius() - hit.distance));
        }
    }
}

static void HandleInput(float dt, Player& player)
{
    if (Input::GetKey(sf::Keyboard::W))
        player.Move(player.GetSpeed() * dt);

    if (Input::GetKey(sf::Keyboard::S))
        player.Move(-player.GetSpeed() * dt);

    if (Input::GetKey(sf::Keyboard::A))
        player.Rotate(-player.GetSpeed() * dt * Math::Deg2Rad);

    if (Input::GetKey(sf::Keyboard::D))
        player.Rotate(player.GetSpeed() * dt * Math::Deg2Rad);
}

void Player::Update(float dt)
{
    HandleInput(dt, *this);
    CheckBounds(m_Position, m_Radius, Game::Get().GetCurrentLevel().GetSize());
    CheckCollision(Game::Get().GetCurrentLevel(), *this);
}