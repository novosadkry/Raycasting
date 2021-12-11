#pragma once
#include <Core.hpp>

class Resource
{
public:
    using ID = uint8_t;

    Resource() = default;
    Resource(const std::fs::path& path)
        : m_Path(path) { }

    virtual ~Resource() = default;

    inline const std::fs::path& GetPath()
    {
        return m_Path;
    }

    friend class ResourceMap;

protected:
    std::fs::path m_Path;

    SERIALIZE_PRIVATE(Resource)
};

class ResourceMap
{
public:
    static Unique<ResourceMap> From(std::fs::path path);
    static void Save(ResourceMap& res, std::fs::path path);

    template <typename T> requires std::is_base_of_v<Resource, T>
    T* Get(Resource::ID id)
    {
        auto type = entt::type_hash<T>::value();
        auto& storage = m_Resources[type];

        if (storage.contains(id))
        {
            if (T* value = dynamic_cast<T*>(storage.at(id).get()))
                return value;
        }

        return nullptr;
    }

    template<typename T> requires std::is_base_of_v<Resource, T>
    void Set(Resource::ID id, Unique<T> res)
    {
        auto type = entt::type_hash<T>::value();
        auto& storage = m_Resources[type];

        storage.insert_or_assign(id, std::move(res));
    }

private:
    using Key = entt::hashed_string::hash_type;
    using Storage = std::unordered_map<Resource::ID, Unique<Resource>>;

    std::unordered_map<Key, Storage> m_Resources;

    SERIALIZE_PRIVATE(ResourceMap)
};