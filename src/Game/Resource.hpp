#pragma once
#include <Core.hpp>

class Resource
{
public:
    using ID = uint8_t;
    virtual ~Resource() = default;

    friend class ResourceMap;

protected:
    std::fs::path m_Path;
};

class ResourceMap
{
public:
    static Unique<ResourceMap> From(std::fs::path path);
    static void Save(ResourceMap& res, std::fs::path path);

    template <typename T> requires std::is_base_of_v<Resource, T>
    T* Get(Resource::ID id)
    {
        auto& storage = m_Resources[typeid(T)];

        try
        {
            if (T* value = dynamic_cast<T*>(storage.at(id).get()))
                return value;

            return nullptr;
        }
        catch (std::exception& e)
        {
            return nullptr;
        }
    }

    template<typename T> requires std::is_base_of_v<Resource, T>
    void Set(Resource::ID id, Unique<T> res)
    {
        auto& storage = m_Resources[typeid(T)];
        storage.insert_or_assign(id, std::move(res));
    }

private:
    using Storage = std::unordered_map<Resource::ID, Unique<Resource>>;
    std::unordered_map<std::type_index, Storage> m_Resources;
};
