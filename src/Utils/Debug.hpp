#pragma once
#include <Rpch.hpp>

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

template<typename T>
std::ostream& operator<<(std::ostream& os, const sf::Vector2<T>& vector)
{
    return os
        << typeid(T).name()
        << "{ " << vector.x
        << ", " << vector.y
        << " }";
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const sf::Vector3<T>& vector)
{
    return os
        << typeid(T).name()
        << "{ " << vector.x
        << ", " << vector.y
        << ", " << vector.z
        << " }";
}
