//
// Created by Matthew McCall on 1/29/22.
//

#ifndef YORK_LAYERLOADER_HPP
#define YORK_LAYERLOADER_HPP

#include <york/Layer.hpp>

#include <string>

typedef york::Layer* (*createLayerFunc)(york::Registry&);

class LayerLoader {
public:
    explicit LayerLoader(std::string layerName, const std::string& createLayerName = "createLayer");
    ~LayerLoader();

    york::Layer* operator->();
    york::Layer& operator*();

private:
    york::Layer* m_layer = nullptr;
    void* m_so = nullptr;
};

#endif // YORK_LAYERLOADER_HPP
