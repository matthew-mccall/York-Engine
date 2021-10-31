#include <fstream>

#include <york/Asset.hpp>

namespace york {

Asset::Asset(const std::string& path, const Type type)
    : filepath(path)
    , type(type)
{
}

unsigned Asset::getSize(const Asset& asset)
{
    return std::filesystem::file_size(asset.filepath);
}

std::shared_ptr<char[]> Asset::load(const Asset& asset)
{
    unsigned size = std::filesystem::file_size(asset.filepath);

    std::shared_ptr<char[]> data(new char[size]);
    std::ifstream file(asset.filepath, std::ios::binary);

    if (!file.is_open()) {
        return nullptr;
    }

    file.read(data.get(), size);
    file.close();

    return data;
}

} // namespace york::asset
