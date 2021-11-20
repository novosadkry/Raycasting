#pragma once

#include "Serialize.hpp"

#include <Game/Level.hpp>
#include <Game/ECS/ECS.hpp>
#include <Game/Resource.hpp>
#include <Game/Grid/Grid.hpp>

namespace cereal
{
    // ---- Generic ----

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
                size.x * size.y * sizeof(Cell)
        ));

        construct(size, std::move(cells));
    }

    // ---- Resource ----

    template<typename Archive>
    void serialize(Archive& archive, Resource& value)
    {
        // TODO
    }

    template<typename Archive>
    void LoadAndConstruct<Resource>::load_and_construct(Archive& archive, cereal::construct<Resource>& construct)
    {
        // TODO
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
