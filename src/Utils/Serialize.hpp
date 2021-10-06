#pragma once
#include <Core.hpp>

#define SERIALIZE_PRIVATE(TYPE)                         \
    template<typename Archive>                          \
    friend void cereal::serialize(Archive&, TYPE&);     \

#define SERIALIZE_PRIVATE_SPLIT(TYPE)                   \
    SERIALIZE_PRIVATE_SAVE(TYPE)                        \
    SERIALIZE_PRIVATE_LOAD(TYPE)                        \

#define SERIALIZE_PRIVATE_SAVE(TYPE)                    \
    template<typename Archive>                          \
    friend void cereal::save(Archive&, const TYPE&);    \

#define SERIALIZE_PRIVATE_LOAD(TYPE)                    \
    template<typename Archive>                          \
    friend void cereal::load(Archive&, TYPE&);          \

class Level;
class Grid;
namespace ECS { class Hierarchy; }

namespace cereal
{
    // ---- Level ----

    template<typename Archive>
    void serialize(Archive&, Level&);

    template<>
    struct LoadAndConstruct<Level>
    {
        template<typename Archive>
        static void load_and_construct(Archive&, cereal::construct<Level>&);
    };

    // ---- Grid ----

    template<typename Archive>
    void serialize(Archive&, Grid&);

    template<>
    struct LoadAndConstruct<Grid>
    {
        template<typename Archive>
        static void load_and_construct(Archive&, cereal::construct<Grid>&);
    };

    // ---- Vector ----

    template<typename Archive, typename T>
    void serialize(Archive&, sf::Vector2<T>&);

    template<typename Archive, typename T>
    void serialize(Archive&, sf::Vector3<T>&);

    template<typename Archive>
    void serialize(Archive&, sf::Color&);

    // ---- ECS ----

    template<typename Archive>
    void save(Archive&, const ECS::Hierarchy&);

    template<typename Archive>
    void load(Archive&, ECS::Hierarchy&);
}
