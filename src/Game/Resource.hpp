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
        try
        {
            if (T* value = dynamic_cast<T*>(m_Resources.at(id).get()))
                return value;

            return nullptr;
        }
        catch (std::exception& e)
        {
            return nullptr;
        }
    }

    Resource* Get(Resource::ID id)
    {
        try
        {
            return m_Resources.at(id).get();
        }
        catch (std::exception& e)
        {
            return nullptr;
        }
    }

    void Set(Resource::ID id, Unique<Resource> res)
    {
        m_Resources.insert_or_assign(id, std::move(res));
    }

private:
    std::unordered_map<Resource::ID, Unique<Resource>> m_Resources;
};
