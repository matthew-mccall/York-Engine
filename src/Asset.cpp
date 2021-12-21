#include <fstream>
#include <unordered_map>

#include "york/Asset.hpp"
#include "york/Log.hpp"

namespace {
    std::unordered_map<std::string, unsigned> s_referenceCount;
    std::unordered_map<std::string, std::vector<char>> s_assetData;
}

namespace york {

Asset::Asset(const std::string& path, Type type)
    : m_filepath(path)
    , m_type(type)
    , m_data(s_assetData[m_filepath.string()])
{
    if (m_type == Type::AUTO) {
        std::string::size_type n = path.find('.');

        if (n == std::string::npos) {
            m_type = Type::RAW;
            return;
        }

        std::string extension = path.substr(n);

        if (extension == "txt") {
            m_type = Type::UTF8;
        } else if (extension == "vert") {
            m_type = Type::SHADER_VERT_GLSL;
        } else if (extension == "frag") {
            m_type = Type::SHADER_FRAG_GLSL;
        } else if (extension == "tesc") {
            m_type = Type::SHADER_TESC_GLSL;
        } else if (extension == "tese") {
            m_type = Type::SHADER_TESE_GLSL;
        } else if (extension == "geom") {
            m_type = Type::SHADER_GEOM_GLSL;
        } else if (extension == "comp") {
            m_type = Type::SHADER_COMP_GLSL;
        }
    }

    s_referenceCount[m_filepath.string()]++;
}

std::reference_wrapper<std::vector<char>> Asset::getData()
{
    if (m_data.empty()) {
        unsigned long size = std::filesystem::file_size(m_filepath);
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

Asset::Type Asset::getType() const
{
    return m_type;
}

Asset::~Asset()
{
    if (--s_referenceCount[m_filepath.string()] == 0) {
        m_data.clear();
    }
}

char& Asset::operator[](std::size_t idx)
{
    return (**this)[idx];
}

std::vector<char>& Asset::operator*()
{
    return getData();
}

std::vector<char>* Asset::operator->()
{
    getData();
    return &m_data;
}

} // namespace york::asset
