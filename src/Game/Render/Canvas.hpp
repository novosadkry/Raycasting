#pragma once
#include <Rpch.hpp>

#include <Utils/Math.hpp>

struct Canvas
{
    static Canvas From(float fov, float distance = 1)
    {
        return Canvas {
            fov,
            tanf(fov / 2) * distance * 2,
            distance,
        };
    }

    float fov;
    float size;
    float distance;
};
