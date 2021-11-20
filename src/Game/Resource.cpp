#include "Resource.hpp"

#include <Utils/Debug.hpp>
#include <Utils/Serialize.impl.hpp>

Unique<Resource> Resource::From(std::fs::path path)
{
    auto res = MakeUnique<Resource>();

    try
    {
        std::ifstream file(path, std::ios_base::binary);
        if (!file) throw std::exception();

        cereal::BinaryInputArchive archive(file);
        archive(*res);
    }
    catch (std::exception& e)
	{
        LOG("Failed to load resource data from " << path)
	}

    return res;
}

void Resource::Save(Resource& res, std::fs::path path)
{
    try
    {
        std::ofstream file(path, std::ios_base::binary);
        if (!file) throw std::exception();

        cereal::BinaryOutputArchive archive(file);
        archive(res);
    }
    catch (std::exception& e)
    {
        LOG("Failed to save resource data to " << path)
    }
}