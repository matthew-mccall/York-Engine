/*
 * BSD 2-Clause License
 *
 * Copyright (c) 2022 Matthew McCall
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <cstring>
#include <unordered_map>
#include <vector>

#include <SDL_rwops.h>

#include "curlpp/Easy.hpp"
#include "curlpp/Options.hpp"

#include "york/Asset.hpp"
#include "york/Log.hpp"
#include "york/Containers.hpp"

namespace {
std::unordered_map<std::string, unsigned> s_referenceCount;
std::unordered_map<std::string, york::Vector<std::uint8_t>> s_assetData;
}

namespace york {

Asset::Asset(const std::string& location, Type type)
    : m_location(location)
    , m_data(s_assetData[m_location])
    , m_type(type)
    , m_size(-1)
{
    if (m_type == Type::AUTO) {
        std::string::size_type n = location.find('.');

        if (n == std::string::npos) {
            m_type = Type::RAW;
            return;
        }

        std::string extension = location.substr(n);

        if (extension == "txt") {
            m_type = Type::UTF8;
        } else if (extension == "xml") {
            m_type = Type::XML;
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

    s_referenceCount[m_location]++;
}

Vector<std::uint8_t>& Asset::getVector()
{
    if (m_data.empty()) {
        SDL_RWops* file = SDL_RWFromFile(m_location.c_str(), "rb");
        if (file == nullptr) {
            log::core::error(SDL_GetError());
            m_data.clear();
            return m_data;
        }

        std::size_t size = m_size < 0 ? SDL_RWsize(file) : m_size;

        if (size < 0) {
            log::core::error(SDL_GetError());
            SDL_RWclose(file);
            m_data.clear();
            return m_data;
        }

        m_data.resize(size);
        YORK_CORE_TRACE("Loading {}!", m_location);

        std::size_t totalSizeRead = 0, sizeRead;

        while (totalSizeRead < size) {
            sizeRead = SDL_RWread(file, m_data.data() + totalSizeRead, sizeof(char), size);
            totalSizeRead += sizeRead;

            if (sizeRead == 0 && (totalSizeRead != size)) {
               YORK_CORE_ERROR(SDL_GetError());
                SDL_RWclose(file);
                m_data.clear();
                return m_data;
            }
        }

        SDL_RWclose(file);
        m_data.push_back('\0');
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
        if (!m_data.empty()) {
            YORK_CORE_TRACE("Unloading {}!", m_location);
        }
        m_data.clear();
    }
}

std::uint8_t& Asset::operator[](std::size_t idx)
{
    return (**this)[idx];
}

Vector<std::uint8_t>& Asset::operator*()
{
    return this->getVector();
}

Vector<std::uint8_t>* Asset::operator->()
{
    this->getVector();
    return &m_data;
}

Vector<std::uint8_t>& NetworkAsset::getVector()
{
    if (m_data.empty()) {
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
    }

    m_data.push_back('\0');

    return m_data;
}

NetworkAsset::NetworkAsset(const std::string& url, Asset::Type type)
    : Asset(url, type)
{
}

StringAsset::StringAsset(const std::string& content, Type type)
    : Asset("StringAsset@" + std::to_string(reinterpret_cast<std::intptr_t>(content.data())), type), m_content(content)
{
}

Vector<std::uint8_t>& StringAsset::getVector()
{
    if (m_data.empty()) {
        m_data = { m_content.begin(), m_content.end() };
    }

    return m_data;
}

} // namespace york::asset
