#include "Collision.hpp"

#include <Game/Game.hpp>
#include <Game/Render/Ray.hpp>

#include <Game/ECS/Components/Collider.hpp>
#include <Game/ECS/Components/Transform.hpp>

using namespace ECS::Components;

namespace ECS::Systems
{
    static void CheckBounds(Transform& transform, const Collider& collider)
    {
        auto bounds = Game::Get().GetCurrentLevel().GetSize();

        if (transform.position.x > bounds.x - collider.radius)
            transform.position.x = bounds.x - collider.radius;

        if (transform.position.y > bounds.y - collider.radius)
            transform.position.y = bounds.y - collider.radius;

        if (transform.position.x < collider.radius)
            transform.position.x = collider.radius;

        if (transform.position.y < collider.radius)
            transform.position.y = collider.radius;
    }

    static void CheckCollision(Transform& transform, const Collider& collider)
    {
        auto& level = Game::Get().GetCurrentLevel();

        for (int i = 0; i < collider.sides; i++)
        {
            float angle = i * (2.0f / collider.sides) * Math::PI;

            Ray hit;
            if (Ray::Cast(level, transform.position, angle, hit))
            {
                if (hit.distance < collider.radius)
                    transform.position -= hit.dir * (collider.radius - hit.distance);
            }
        }
    }

    void Collision::Run(float dt)
    {
        auto& reg = Game::Get().GetCurrentLevel().GetRegistry();
        auto group = reg.group<Collider>(entt::get<Transform>);

        group.each([](Collider& collider, Transform& transform)
        {
            CheckBounds(transform, collider);
            CheckCollision(transform, collider);
        });
    }
}
