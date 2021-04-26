#include "Player.hpp"

#define _USE_MATH_DEFINES
#include <cmath>

void Player::SetDirection(float newDir)
{
    m_Direction = newDir - floor(newDir / 360.0f) * 360.0f;
}

void Player::Move(float amount)
{
    float x = cos(m_Direction / 180.0f * M_PI);
    float y = sin(m_Direction / 180.0f * M_PI);

    m_Position.x += x * amount;
    m_Position.y += y * amount;
}
