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

class Grid;
class Level;
struct Cell;

class Texture;
class Resource;
class ResourceMap;

namespace ECS { class Hierarchy; }

namespace std::filesystem
{
    template <class Archive>
    std::string save_minimal(const Archive&, const path&);

    template <class Archive>
    void load_minimal(const Archive&, path&, const std::string&);
}

namespace cereal
{
    // ---- Binary ----

    template<typename T, typename Archive> requires traits::is_text_archive<Archive>::value
    void save(Archive&, const cereal::BinaryData<T*>&);

    template<typename T, typename Archive> requires traits::is_text_archive<Archive>::value
    void load(Archive&, cereal::BinaryData<T*>&);

    // ---- Level ----

    template<typename Archive>
    void serialize(Archive&, Level&);

    // ---- Grid ----

    template<typename Archive>
    void serialize(Archive&, Grid&);

    template<>
    struct LoadAndConstruct<Grid>
    {
        template<typename Archive>
        static void load_and_construct(Archive&, cereal::construct<Grid>&);
    };

    // ---- Resource ----

    template<typename Archive>
    void serialize(Archive&, ResourceMap&);

    template<typename Archive>
    void serialize(Archive&, Resource&);

    template<typename Archive>
    void serialize(Archive&, Texture&);

    template<>
    struct LoadAndConstruct<Texture>
    {
        template<typename Archive>
        static void load_and_construct(Archive&, cereal::construct<Texture>&);
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
    void save(Archive&, const entt::registry&);

    template<typename Archive>
    void load(Archive&, ECS::Hierarchy&);

    template<typename Archive>
    void load(Archive&, entt::registry&);
}
