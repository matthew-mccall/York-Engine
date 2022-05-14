//
// Created by Matthew McCall on 5/13/22.
//

#ifndef YORK_VERTEX_HPP
#define YORK_VERTEX_HPP

#include <array>

#include <glm/glm.hpp>

namespace york {

    using Vec2 = glm::vec2;
    using Vec3 = glm::vec3;

    struct Vertex {
        Vec2 position;
        Vec3 color;

    template <typename T>
    T static getBindingDescription();

    template <typename T>
    std::array<T, 2> static getAttributeDescriptions();

    };


}

#endif // YORK_VERTEX_HPP
