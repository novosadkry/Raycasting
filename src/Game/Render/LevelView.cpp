#include "LevelView.hpp"

#include <Game/Game.hpp>
#include <Utils/Math.hpp>
#include <Utils/Debug.hpp>

// Traces for intersections
static float Trace(Level& level, Player& player, float angle)
{
    sf::Vector2i cell = level.GetGridCellFromPos(player.GetPosition());

    float cellWidth = (float) level.GetSize().x / level.GetGrid().SizeX();
    float cellHeight = (float) level.GetSize().y / level.GetGrid().SizeY();

    angle = angle / 180.0f * PI;

    float dirX = cos(angle);
    float dirY = sin(angle);

    int stepX;
    int stepY;

    float deltaX;
    float deltaY;

    float rayX;
    float rayY;

    if (dirX < 0)
    {
        stepX = -1;
        deltaX = -cellWidth / cos(angle);
        rayX = player.GetPosition().x - cell.x * cellWidth;
    }

    else
    {
        stepX = 1;
        deltaX = cellWidth / cos(angle);
        rayX = (cell.x + 1) * cellWidth - player.GetPosition().x;
    }

    if (dirY < 0)
    {
        stepY = -1;
        deltaY = -cellHeight / sin(angle);
        rayY = player.GetPosition().y - cell.y * cellHeight;
    }

    else
    {
        stepY = 1;
        deltaY = cellHeight / sin(angle);
        rayY = (cell.y + 1) * cellHeight - player.GetPosition().y;
    }

    for (int i = 0; i < 128; i++)
    {
        if (rayX < rayY)
        {
            rayX += deltaX;
            cell.x += stepX;
        }

        else
        {
            rayY += deltaY;
            cell.y += stepY;
        }

        if (level.GetGrid().Get(cell) == Wall)
            return std::min(rayX, rayY);
    }

    return 0;
}

static void RenderView(Level& level, Player& player, float fov)
{
    LOG(Trace(level, player, player.GetDirection()));
}

void LevelView::Render(float dt)
{
    RenderView(Game::Get().GetCurrentLevel(), *m_Player, m_FOV);
}