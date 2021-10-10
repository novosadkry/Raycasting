#pragma once

#include "Serialize.hpp"

#include <Game/Level.hpp>
#include <Game/ECS/ECS.hpp>
#include <Game/Grid/Grid.hpp>

namespace cereal
{
    // ---- Level ----

    template<typename Archive>
    void serialize(Archive& archive, Level& value)
    {
        archive(value.m_Size, value.m_Grid, value.m_Hierarchy);
    }

    template<typename Archive>
    void LoadAndConstruct<Level>::load_and_construct(Archive& archive, cereal::construct<Level>& construct)
    {
        sf::Vector2i size;
        Unique<Grid> grid;

        archive(size, grid);
        construct(size, std::move(*grid));
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

    template<typename Archive> requires traits::is_text_archive<Archive>::value
    void save(Archive& archive, const cereal::BinaryData<Cell*>& value)
    {
        archive.saveBinaryValue(value.data, value.size);
    }

    template<typename Archive> requires traits::is_text_archive<Archive>::value
    void load(Archive& archive, cereal::BinaryData<Cell*>& value)
    {
        archive.loadBinaryValue(value.data, value.size);
    }

    template<typename Archive>
    void LoadAndConstruct<Grid>::load_and_construct(Archive& archive, cereal::construct<Grid>& construct)
    {
        sf::Vector2i size;
        std::vector<Cell> cells;

        archive(size, cells);
        construct(size, std::move(cells));
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
        using namespace ECS::Components;

        entt::snapshot(value.m_Registry)
            .entities(archive)
            .template component<Tag>(archive)
            .template component<Light>(archive)
            .template component<Player>(archive)
            .template component<Collider>(archive)
            .template component<Transform>(archive);
    }

    template<typename Archive>
    void load(Archive& archive, ECS::Hierarchy& value)
    {
        using namespace ECS::Components;

        entt::snapshot_loader(value.m_Registry)
            .entities(archive)
            .template component<Tag>(archive)
            .template component<Light>(archive)
            .template component<Player>(archive)
            .template component<Collider>(archive)
            .template component<Transform>(archive);
    }
}
