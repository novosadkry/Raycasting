#pragma once
#include <Core.hpp>

template<typename T> using Unique<T> = std::unique_ptr<T>;
template<typename T> using Shared<T> = std::shared_ptr<T>;

template<typename T, typename... Args>
inline Unique<T> MakeUnique(Args&&... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T, typename... Args>
inline Shared<T> MakeShared(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}
