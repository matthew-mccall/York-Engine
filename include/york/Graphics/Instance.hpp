#if !defined(YORK_GRAPHICS_INSTANCE_HPP)
#define YORK_GRAPHICS_INSTANCE_HPP

#include <string>

#include <vulkan/vulkan.hpp>

#include "Handle.hpp"
#include "Window.hpp"

namespace york::graphics {

class Instance : public Handle<vk::Instance> {
public:
    Instance(Window& window);

protected:
    bool createImpl() override;
    void destroyImpl() override;

private:
    Window& m_window;
};

}

#endif
