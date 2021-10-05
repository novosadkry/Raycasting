#pragma once

#include "Serialize.hpp"

#include <Game/Level.hpp>
#include <Game/Grid/Grid.hpp>
#include <Game/ECS/Hierarchy.hpp>

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
        archive(value.m_Size, value.m_Cells);
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

    template<typename Archive, typename T>
    void serialize(Archive& archive, sf::Color& value)
    {
        archive(value.r, value.g, value.b, value.a);
    }

    // ---- ECS ----

    template<typename Archive>
    void save(Archive& archive, const ECS::Hierarchy& value)
    {
        // TODO
    }

    template<typename Archive>
    void load(Archive& archive, ECS::Hierarchy& value)
    {
        // TODO
    }
}
