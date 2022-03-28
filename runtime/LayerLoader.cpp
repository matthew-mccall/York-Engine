//
// Created by Matthew McCall on 1/29/22.
//

#include <SDL2/SDL_loadso.h>

#include "york/Log.hpp"
#include "york/Registry.hpp"
#include "york/Config.hpp"

#include "LayerLoader.hpp"

LayerLoader::LayerLoader(std::string layerName, const std::string& createLayerName)
{

    std::string platformName {YORK_PLATFORM };

    if (platformName == std::string { "Windows" }) {
        layerName += ".dll";
    } else if (platformName == std::string { "Darwin" }) {
        layerName = "lib" + layerName + ".dylib";
    } else {
        layerName = "lib" + layerName + ".so";
    }

    m_so = SDL_LoadObject(layerName.c_str());

    if (m_so == nullptr) {
        YORK_CORE_ERROR(SDL_GetError());
        YORK_CORE_ERROR("Failed to load {} layer!", layerName);
        return;
    }

    void* createLayerPtr = SDL_LoadFunction(m_so, createLayerName.c_str());

    if (createLayerPtr == nullptr) {
        YORK_CORE_ERROR(SDL_GetError());
        YORK_CORE_ERROR("Failed to find {}() to create {}!", createLayerName, layerName);
        return;
    }

    auto createLayer = reinterpret_cast<createLayerFunc>(createLayerPtr);

    m_layer = createLayer(york::Registry::getInstance());
}

LayerLoader::~LayerLoader()
{
    delete m_layer;

    if (m_so) {
        SDL_UnloadObject(m_so);
    }
}

york::Layer* LayerLoader::operator->()
{
    return m_layer;
}
york::Layer& LayerLoader::operator*()
{
    return *m_layer;
}
