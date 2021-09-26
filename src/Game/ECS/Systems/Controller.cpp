#include "Controller.hpp"

#include <Game/Game.hpp>

#include <Game/ECS/Components/Player.hpp>
#include <Game/ECS/Components/Transform.hpp>

using namespace ECS::Components;

namespace ECS::Systems
{
    static void SetRotation(Transform& transform, float newDir)
    {
        transform.rotation = newDir - floor(newDir / Math::PI_2) * Math::PI_2;
    }

    static void Move(Transform& transform, float amount)
    {
        sf::Vector2f dir = Math::Angle2Vector(transform.rotation);
        transform.position += dir * amount;
    }

    static void Strafe(Transform& transform, float amount)
    {
        sf::Vector2f dir = Math::Angle2Vector(transform.rotation + Math::PI / 2);
        transform.position += dir * amount;
    }

    static void Rotate(Transform& transform, float amount)
    {
        SetRotation(transform, transform.rotation + amount);
    }

    static void HandleInput(Player& player, Transform& transform, float dt)
    {
        if (Input::GetKey(Keyboard::W))
            Move(transform, player.speed * dt);

        if (Input::GetKey(Keyboard::S))
            Move(transform, -player.speed * dt);

        if (Input::GetKey(Keyboard::A))
        {
            Input::GetKey(Keyboard::LShift)
                ? Strafe(transform, -player.speed * dt)
                : Rotate(transform, -player.speed * dt * Math::Deg2Rad);
        }

        if (Input::GetKey(Keyboard::D))
        {
            Input::GetKey(Keyboard::LShift)
                ? Strafe(transform, player.speed * dt)
                : Rotate(transform, player.speed * dt * Math::Deg2Rad);
        }
    }

    void Controller::Run(float dt)
    {
        auto& reg = Game::Get().GetCurrentLevel().GetRegistry();
        auto group = reg.group<Player>(entt::get<Transform>);

        group.each([dt](Player& player, Transform& transform)
        {
            HandleInput(player, transform, dt);
        });
    }
}
