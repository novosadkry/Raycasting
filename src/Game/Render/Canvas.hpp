#pragma once

struct Canvas
{
    static Canvas From(float fov, float distance = 1)
    {
        return Canvas {
            fov,
            tan(fov / 2) * distance * 2,
            distance,
        };
    }

    float fov;
    float size;
    float distance;
};
