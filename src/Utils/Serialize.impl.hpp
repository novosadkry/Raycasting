#pragma once

#include "Serialize.hpp"

#include <Game/Level.hpp>
#include <Game/ECS/ECS.hpp>
#include <Game/Resource.hpp>
#include <Game/Grid/Grid.hpp>
#include <Game/Render/Texture.hpp>

CEREAL_REGISTER_TYPE(Texture)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Resource, Texture)

namespace std::filesystem
{
    template <class Archive>
    std::string save_minimal(const Archive& archive, const path& value)
    {
        return value.string();
    }

    template <class Archive>
    void load_minimal(const Archive& archive, path& out, const std::string& str)
    {
       out = path(str);
    }
}

namespace cereal
{
    // ---- Binary ----

    template<typename T, typename Archive> requires traits::is_text_archive<Archive>::value
    void save(Archive& archive, const cereal::BinaryData<T*>& value)
    {
        archive.saveBinaryValue(value.data, value.size);
    }

    template<typename T, typename Archive> requires traits::is_text_archive<Archive>::value
    void load(Archive& archive, cereal::BinaryData<T*>& value)
    {
        archive.loadBinaryValue(value.data, value.size);
    }

    // ---- Level ----

    template<typename Archive>
    void serialize(Archive& archive, Level& value)
    {
        archive(value.m_Name, value.m_Size, value.m_Grid, value.m_Hierarchy);
    }

    // ---- Grid ----

    template<typename Archive>
    void serialize(Archive& archive, Grid& value)
    {
        archive(value.m_Size);
        archive(
            cereal::binary_data(
                value.GetCells().data(),
                value.m_Size.x * value.m_Size.y * sizeof(Cell)
        ));
    }

    template<typename Archive>
    void LoadAndConstruct<Grid>::load_and_construct(Archive& archive, cereal::construct<Grid>& construct)
    {
        sf::Vector2i size;
        archive(size);

        std::vector<Cell> cells(size.x * size.y);
        archive(
            cereal::binary_data(
                cells.data(),
                cells.size() * sizeof(Cell)
        ));

        construct(size, std::move(cells));
    }

    // ---- Resource ----

    template<typename Archive>
    void serialize(Archive& archive, ResourceMap& value)
    {
        archive(value.m_Resources);
    }

    template<typename Archive>
    void serialize(Archive& archive, Resource& value)
    {
        archive(value.m_Path);
    }

    template<typename Archive>
    void serialize(Archive& archive, Texture& value)
    {
        archive(cereal::base_class<Resource>(&value));

        if (value.m_Path.empty())
        {
            sf::Image img = value.m_Handle.copyToImage();

            archive(img.getSize());
            archive(
                cereal::binary_data(
                    img.getPixelsPtr(),
                    img.getSize().x * img.getSize().y * 4
            ));
        }
    }

    template<typename Archive>
    void LoadAndConstruct<Texture>::load_and_construct(Archive& archive, cereal::construct<Texture>& construct)
    {
        Resource res;
        archive(cereal::base_class<Resource>(&res));

        if (res.GetPath().empty())
        {
            sf::Vector2u size;
            archive(size);

            std::vector<sf::Uint8> pixels(size.x * size.y * 4);
            archive(
                cereal::binary_data(
                    pixels.data(),
                    pixels.size()
            ));

            construct(size, pixels.data());
        }

        else
            construct(res.GetPath());
    }

    // ---- SFML ----

    template<typename Archive, typename T>
    void serialize(Archive& archive, sf::Vector2<T>& value)
    {
        archive(value.x, value.y);
    }

    template<typename Archive, typename T>
    void serialize(Archive& archive, sf::Vector3<T>& value)
    {
        archive(value.x, value.y, value.z);
    }

    template<typename Archive>
    void serialize(Archive& archive, sf::Color& value)
    {
        archive(value.r, value.g, value.b, value.a);
    }

    // ---- ECS ----

    template<typename Archive>
    void save(Archive& archive, const ECS::Hierarchy& value)
    {
        archive(value.m_Registry);
    }

    template<typename Archive>
    void save(Archive& archive, const entt::registry& value)
    {
        save(archive, value, ECS::AllComponents{});
    }

    template<typename Archive, typename... Component>
    void save(Archive& archive, const entt::registry& value, ECS::ComponentGroup<Component...>)
    {
        entt::snapshot(value)
            .entities(archive)
            .template component<Component...>(archive);
    }

    template<typename Archive>
    void load(Archive& archive, ECS::Hierarchy& value)
    {
        archive(value.m_Registry);
    }

    template<typename Archive>
    void load(Archive& archive, entt::registry& value)
    {
        load(archive, value, ECS::AllComponents{});
    }

    template<typename Archive, typename... Component>
    void load(Archive& archive, entt::registry& value, ECS::ComponentGroup<Component...>)
    {
        entt::snapshot_loader(value)
            .entities(archive)
            .template component<Component...>(archive);
    }
}
