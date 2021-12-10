#pragma once
#include <Core.hpp>

#include <Game/Resource.hpp>

class Texture : public Resource
{
public:
    Texture(const std::fs::path& path)
        : Resource(path), m_Handle()
    {
        m_Handle.loadFromFile(path.string());
    }

    Texture(const void* data, std::size_t size)
        : m_Handle()
    {
        m_Handle.loadFromMemory(data, size);
    }

    Texture(sf::Vector2u size, sf::Uint8* pixels)
        : m_Handle()
    {
        sf::Image img;
        img.create(size.x, size.y, pixels);

        m_Handle.loadFromImage(img);
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

    SERIALIZE_PRIVATE(Texture)
};
