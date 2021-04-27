#include "LevelView.hpp"

#include <Game/Game.hpp>
#include <Utils/Math.hpp>
#include <Utils/Debug.hpp>

// Traces for horizontal intersections
static float TraceVertically(Level& level, Player& player, float angle)
{
    sf::Vector2i cell = level.GetGridCellFromPos(player.GetPosition());

    float cellWidth = (float) level.GetSize().x / level.GetGrid().SizeX();
    float cellHeight = (float) level.GetSize().y / level.GetGrid().SizeY();

    angle = angle / 180.0f * PI;

    float stepY = cellHeight;
    float stepX = stepY / tan(angle);

    float nearY = (cell.y + 1) * cellHeight - player.GetPosition().y;
    float nearX = nearY / tan(angle);

    float startY = player.GetPosition().y;
    float startX = player.GetPosition().x;

    float rayY = startY + nearY;
    float rayX = startX + nearX;

    for (int i = 0; i < 2; i++)
    {
        Cell cell = level.GetGrid().Get(level.GetGridCellFromPos({rayX, rayY}));

        if (cell == Wall)
        {
            float dY = rayY - startY;
            float dX = rayX - startX;
            return dX * dX + dY * dY;
        }

        rayY += stepY;
        rayX += stepX;
    }

    return 0;
}

static void RenderView(Level& level, Player& player, float fov)
{
    LOG(TraceVertically(level, player, player.GetDirection()));
}

void LevelView::Render(float dt)
{
    RenderView(Game::Get().GetCurrentLevel(), *m_Player, m_FOV);
}