#pragma once
#include <Core.hpp>

#include <Game/Resource.hpp>

class Texture : public Resource
{
public:
    Texture(std::fs::path path)
        : m_Handle()
    {
        m_Handle.loadFromFile(path.string());
        m_Path = std::move(path);
    }

    sf::Vector2u GetSize()
    {
        return m_Handle.getSize();
    }

    sf::Texture& GetHandle()
    {
        return m_Handle;
    }

private:
    sf::Texture m_Handle;
};
