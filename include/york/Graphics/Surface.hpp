//
// Created by Matthew McCall on 12/21/21.
//

#ifndef YORK_GRAPHICS_SURFACE_HPP
#define YORK_GRAPHICS_SURFACE_HPP

#include <vulkan/vulkan.hpp>

#include <york/Window.hpp>

#include "Handle.hpp"
#include "Instance.hpp"

namespace york::graphics {

/**
 * Handle wrapper for Vulkan Surface
 */
class Surface : public Handle<vk::SurfaceKHR> {
public:
    explicit Surface(Instance& instance, Window& window);

private:
    bool createImpl() override;
    void destroyImpl() override;

    Instance& m_instance;
    Window& m_window;
};

}

#endif // YORK_GRAPHICS_SURFACE_HPP
