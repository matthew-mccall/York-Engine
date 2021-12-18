#include <fstream>
#include <unordered_map>

#include "york/Asset.hpp"
#include "york/Log.hpp"

namespace york {

Asset::Asset(const std::string& path, Type type)
    : m_filepath(path)
    , m_type(type)
{
}

unsigned long Asset::getSize() const
{
    return std::filesystem::file_size(m_filepath);
}

std::reference_wrapper<std::vector<char>> Asset::getData()
{
    if (m_data.empty()) {
        log::core::debug("Loading {}!", m_filepath.string());

        unsigned long size = this->getSize();
        m_data.resize(size);

        std::ifstream file(m_filepath, std::ios::binary);

        if (file.is_open()) {
            file.read(m_data.data(), static_cast<long>(size));
        }

    }

    return m_data;
}

std::reference_wrapper<std::vector<char>> Asset::getDataStatic(Asset asset)
{
    return asset.getData();
}

} // namespace york::asset
