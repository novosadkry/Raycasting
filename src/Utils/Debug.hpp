#pragma once

#include <cassert>
#include <iostream>

#ifndef NDEBUG
    #define LOG(x) std::cout    \
        << "[" << __FILE__      \
        << ":" << __func__      \
        << ":" << __LINE__      \
        << "] " << (x)          \
        << std::endl;
    #define ASSERT(x) assert((x));
#else
    #define LOG(x)
    #define ASSERT(x)
#endif

namespace sf
{
    template<typename T>
    class Vector2;

    template<typename T>
    class Vector3;

    template<typename T>
    std::ostream& operator<<(std::ostream& os, const Vector2<T>& vector)
    {
        return os
            << typeid(T).name()
            << "{ " << vector.x
            << ", " << vector.y
            << " }";
    }

    template<typename T>
    std::ostream& operator<<(std::ostream& os, const Vector3<T>& vector)
    {
        return os
            << typeid(T).name()
            << "{ " << vector.x
            << ", " << vector.y
            << ", " << vector.z
            << " }";
    }
};