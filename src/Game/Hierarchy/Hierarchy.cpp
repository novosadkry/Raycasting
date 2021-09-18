#include "Hierarchy.hpp"

void Hierarchy::Serialize(std::ostream &stream) const
{
    int count = 0;
    for (auto& [type, storage] : m_Objects)
        count += storage.size();

    ::Serialize<int>(count, stream);

    for (auto& [type, storage] : m_Objects)
    {
        for (auto& obj : storage)
            ::Serialize<Object>(*obj, stream);
    }
}

Unique<Hierarchy> Hierarchy::Deserialize(std::istream &stream)
{
    auto hierarchy = MakeUnique<Hierarchy>();

    int count = ::Deserialize<int>(stream);

    for (int i = 0; i < count; i++)
    {
        Unique<Object> obj = ::Deserialize<Object>(stream);
        hierarchy->AddObject(std::move(obj));
    }

    return hierarchy;
}