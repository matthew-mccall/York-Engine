#include <fstream>

#include <york/Asset.hpp>

namespace york::asset {

Asset::Asset(const std::string& path, const Type type)
    : filepath(path)
    , type(type)
{
}

unsigned getSize(const Asset& asset)
{
    return std::filesystem::file_size(asset.filepath);
}

async::Future<std::optional<std::shared_ptr<char[]>>> load(const Asset& asset)
{
    return async::dispatch<std::shared_ptr<char[]>>([=]() -> std::shared_ptr<char[]> {
        unsigned size = std::filesystem::file_size(asset.filepath);

        std::shared_ptr<char[]> data(new char[size]);
        std::ifstream file(asset.filepath, std::ios::binary);

        if (!file.is_open()) {
            return nullptr;
        }

        file.read(data.get(), size);
        file.close();

        return data;
    });
}

} // namespace york::asset
