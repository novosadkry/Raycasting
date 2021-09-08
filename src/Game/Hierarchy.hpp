#pragma once
#include <Core.hpp>

#include <Game/Render/Object.hpp>

class Hierarchy
{
public:
    using Storage = std::vector<Shared<Object>>;
    using ObjectMap = std::unordered_map<std::type_index, Storage>;

    struct Iterator // This is like the worst code I've written so far
    {
        using Key = std::type_index;
        using Value = Shared<Object>;

        using ValueType = std::pair<const Key, Value&>;
        using IteratorCategory = std::forward_iterator_tag;

        explicit Iterator(
            Hierarchy::ObjectMap::iterator begin,
            Hierarchy::ObjectMap::iterator end
        ) : mCurrent(begin), mEnd(end)
        {
            if (begin == end)
                return;

            auto& [key, value] = *mCurrent;
            sCurrent = value.begin();
            sEnd = value.end();
        }

        Iterator& operator++()
        {
            if (++sCurrent == sEnd)
            {
                if (++mCurrent != mEnd)
                {
                    auto& [key, value] = *mCurrent;
                    sCurrent = value.begin();
                    sEnd = value.end();
                }
            }

            return *this;
        }

        Iterator operator++(int)
        {
            auto tmp = *this;
            ++*this;
            return tmp;
        }

        ValueType operator*()
        {
            return { mCurrent->first, *sCurrent };
        }

        bool operator==(const Iterator& other)
        {
            return mCurrent == other.mCurrent
                || sCurrent == other.sCurrent;
        };

        bool operator!=(const Iterator& other)
        {
            return !(*this == other);
        };

        Hierarchy::ObjectMap::iterator mCurrent;
        Hierarchy::ObjectMap::iterator mEnd;

        Hierarchy::Storage::iterator sCurrent;
        Hierarchy::Storage::iterator sEnd;
    };

    template<typename T, typename... Args>
    void AddObject(Args&& ...args)
    {
        auto& storage = m_Objects[typeid(T)];
        storage.emplace_back(MakeShared<T>(std::forward<Args>(args)...));
    }

    template<typename T>
    Shared<T> GetObject()
    {
        auto& storage = m_Objects[typeid(T)];

        if (!storage.empty())
            return std::static_pointer_cast<T>(storage[0]);

        return nullptr;
    }

    template<typename T>
    Storage& GetObjects()
    {
        return m_Objects[typeid(T)];
    }

    template<typename T>
    void RemoveObject(T obj)
    {
        auto& storage = m_Objects[typeid(T)];
        storage.erase(std::remove(storage.begin(), storage.end(), obj), storage.end());
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

    Iterator begin()
    {
        return Iterator(m_Objects.begin(), m_Objects.end());
    }

    Iterator end()
    {
        return Iterator(m_Objects.end(), m_Objects.end());
    }

private:
    ObjectMap m_Objects;
};
