#pragma once
#include <Core.hpp>

class Resource
{
public:
    using ID = uint8_t;

    friend class ResourceMap;
};

class ResourceMap
{
public:
    static Unique<ResourceMap> From(std::fs::path path);
    static void Save(ResourceMap& res, std::fs::path path);

    template <typename T> requires std::is_base_of_v<Resource, T>
    T* Get(Resource::ID id);

    Resource* Get(Resource::ID id);
    void Set(Resource::ID id, Unique<Resource> res);

private:
    std::unordered_map<Resource::ID, Unique<Resource>> m_Textures;
};
