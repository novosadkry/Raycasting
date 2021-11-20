#pragma once
#include <Core.hpp>

class Resource
{
public:
    using ID = uint8_t;

    static Unique<Resource> From(std::fs::path path);
    static void Save(Resource& res, std::fs::path path);

    sf::Texture& GetTexture(ID id);
    void SetTexture(ID id, sf::Texture texture);

private:
    std::unordered_map<ID, sf::Texture> m_Textures;
};
