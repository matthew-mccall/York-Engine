#include <unordered_map>
#include <cstring>

#include "curlpp/Easy.hpp"
#include "curlpp/Options.hpp"
#include "SDL_rwops.h"

#include "york/Asset.hpp"
#include "york/Log.hpp"

namespace {
std::unordered_map<std::string, unsigned> s_referenceCount;
std::unordered_map<std::string, std::vector<char>> s_assetData;
}

namespace york {

Asset::Asset(const std::string& location, Type type, Source source)
    : m_location(location)
    , m_data(s_assetData[m_location])
    , m_type(type)
    , m_source(source)
    , m_size(-1)
{
    if (m_source == Source::FILE) {
        if (m_type == Type::AUTO) {
            std::string::size_type n = location.find('.');

            if (n == std::string::npos) {
                m_type = Type::RAW;
                return;
            }

            std::string extension = location.substr(n);

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
    }

    s_referenceCount[m_location]++;
}

std::optional<std::reference_wrapper<std::vector<char>>> Asset::getData()
{
    if (m_data.empty()) {
        switch (m_source) {
        case Source::FILE: {
            SDL_RWops* file = SDL_RWFromFile(m_location.c_str(), "rb");
            if (file == nullptr) {
                log::core::error(SDL_GetError());
                m_data.clear();
                return std::nullopt;
            }

            std::size_t size = m_size == -1 ? SDL_RWsize(file) : m_size;
            m_data.resize(size);
            log::core::debug("Loading {}!", m_location);

            if (size < 0) {
                log::core::error(SDL_GetError());
                SDL_RWclose(file);
                m_data.clear();
                return std::nullopt;
            }

            std::size_t totalSizeRead = 0, sizeRead;

            while (totalSizeRead < size) {
                sizeRead = SDL_RWread(file, m_data.data() + totalSizeRead, sizeof(char), size);
                totalSizeRead += sizeRead;

                if (sizeRead == 0 && (totalSizeRead != size)) {
                    log::core::error(SDL_GetError());
                    SDL_RWclose(file);
                    m_data.clear();
                    return std::nullopt;
                }
            }

            SDL_RWclose(file);
            m_data.push_back('\0');
        } break;
        case Source::NETWORK: {
            curlpp::Easy req;
            req.setOpt<curlpp::Options::Url>(m_location);
            req.setOpt<curlpp::Options::WriteFunction>([&](char* ptr, size_t size, size_t nmemb) -> size_t {
                size_t realSize = size * nmemb;
                size_t lastSize = m_data.size();
                m_data.resize(lastSize + realSize);
                std::memcpy(m_data.data() + lastSize, ptr, realSize);
                return m_data.size() - lastSize;
            });
            log::core::debug("Loading {}!", m_location);
            req.perform();

            m_data.push_back('\0');
        } break;
        case Source::MEMORY:
            break;
        }
    }

    return m_data;
}

Asset::Type Asset::getType() const
{
    return m_type;
}

Asset::~Asset()
{
    if (--s_referenceCount[m_location] == 0) {
        log::core::debug("Unloading {}!", m_location);
        m_data.clear();
    }
}

char& Asset::operator[](std::size_t idx)
{
    return (***this).get()[idx];
}

std::optional<std::reference_wrapper<std::vector<char>>> Asset::operator*()
{
    return getData();
}

std::vector<char>* Asset::operator->()
{
    getData();
    return &m_data;
}

} // namespace york::asset
