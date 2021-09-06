#pragma once
#include <Core.hpp>

#include <Game/Render/Object.hpp>

class Hierarchy
{
public:
    using ObjectMap = std::unordered_multimap<std::type_index, Shared<Object>>;

    template<typename T, typename... Args>
    void AddObject(Args&& ...args)
    {
        m_Objects.emplace(typeid(T), MakeShared<T>(std::forward<Args>(args)...));
    }

    template<typename T>
    Weak<T> GetObject()
    {
        auto it = m_Objects.find(typeid(T));

        for (; it != m_Objects.end(); ++it)
        {
            auto& obj = std::get<Shared<Object>>(*it);
            return std::static_pointer_cast<T>(obj);
        }

        return {};
    }

    template<typename T>
    std::vector<Weak<T>> GetObjects()
    {
        std::vector<Weak<T>> refs;
        auto it = m_Objects.find(typeid(T));

        for (; it != m_Objects.end(); ++it)
        {
            auto& obj = std::get<Shared<Object>>(*it);
            refs.push_back(std::static_pointer_cast<T>(obj));
        }

        return refs;
    }

    template<typename T>
    void RemoveObject(T obj)
    {
        m_Objects.erase(std::remove(m_Objects.begin(), m_Objects.end(), obj), m_Objects.end());
    }

    template<typename T>
    void RemoveObjects()
    {
        m_Objects.erase(typeid(T));
    }

    void Clear()
    {
        m_Objects.clear();
    }

    ObjectMap::iterator begin()
    {
        return m_Objects.begin();
    }

    ObjectMap::iterator end()
    {
        return m_Objects.end();
    }

private:
    ObjectMap m_Objects;
};
