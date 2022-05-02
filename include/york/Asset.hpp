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

#if !defined(YORK_ASSET_HPP)
#define YORK_ASSET_HPP

#include <string>
#include <vector>

namespace york {

/**
 * An Asset is any file that can be loaded by the engine.
 */
class Asset {
public:

    /**
     * @brief The type of Asset.
     *
     * This can allow the engine to automatically decode the file upon loading.
     */
    enum class Type {
        IMG_PNG,
        SHADER_FRAG_GLSL,
        SHADER_FRAG_SPIRV,
        SHADER_VERT_GLSL,
        SHADER_VERT_SPIRV,
        SHADER_TESC_GLSL,
        SHADER_TESC_SPIRV,
        SHADER_TESE_GLSL,
        SHADER_TESE_SPIRV,
        SHADER_GEOM_GLSL,
        SHADER_GEOM_SPIRV,
        SHADER_COMP_GLSL,
        SHADER_COMP_SPIRV,
        XML,
        UTF8,
        RAW,
        AUTO /**< Infer asset type from file extension */
    };

    /**
     * @brief Creates an Asset handle, but does not load it.
     *
     * @param location A relative or absolute filepath to the asset, or a URL to a network location.
     * @param type The type of asset
     */
    explicit Asset(const std::string& location, Type type = Type::AUTO);

    /**
     * Gets the type of the asset
     * @return The type of the asset
     */
    [[nodiscard]] Type getType() const;

    /**
     * Returns the loaded data, if available.
     *
     * This may attempt to load the data on the spot if it is the first time called, previously failed, or the data was unloaded.
     * @return
     */
    virtual std::vector<std::uint8_t>& getVector();

    std::vector<std::uint8_t>& operator*();
    std::vector<std::uint8_t>* operator->();

    std::uint8_t & operator[](std::size_t idx);

    virtual ~Asset();

protected:
    std::string m_location;
    std::vector<std::uint8_t>& m_data;
    Type m_type;
    int m_size;
};

/**
 * An asset to be fetched from the internet
 */
class NetworkAsset : public Asset {
public:
    /**
     * Creates an asset that can be fetched from the internet.
     *
     * Assumes a working internet connection
     * @param url The URL to fetch from
     * @param type The type of document
     */
    explicit NetworkAsset(const std::string& url, Type type = Type::UTF8);
    std::vector<std::uint8_t>& getVector() override;
};

/**
 * An that represents and in-memory string
 */
class StringAsset : public Asset {
public:
    /**
     * Creates a StringAsset
     * @param content The string to manage
     * @param type The type of string
     */
    explicit StringAsset(const std::string& content, Type type = Type::UTF8);
    std::vector<std::uint8_t>& getVector() override;

private:
    std::string m_content;
};

/**
 * An asset that represents an in-memory byte array
 * @tparam N The number of bytes
 */
template <std::size_t N>
class BinaryAsset : public Asset {
public:
    /**
     * Creates a BinaryAsset
     * @param content The array of bytes to manage
     * @param type The type of data
     */
    explicit BinaryAsset(const std::array<std::uint8_t, N> content, Type type = Type::RAW)
        : Asset("BinaryAsset@" + std::to_string(reinterpret_cast<std::intptr_t>(content.data())), type), m_content(std::move(content))
    {
    }

    std::vector<std::uint8_t>& getVector() override
    {
      if (m_data.empty()) {
        m_data = { m_content.begin(), m_content.end() };
      }

      return m_data;
    }

private:
    std::array<std::uint8_t, N> m_content;
};

} // namespace york::asset

#endif // YORK_ASSET_HPP
