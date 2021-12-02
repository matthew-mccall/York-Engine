#if !defined(YORK_GRAPHICS_INSTANCE_HPP)
#define YORK_GRAPHICS_INSTANCE_HPP

#include <string>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "Handle.hpp"
#include "Window.hpp"

namespace york::graphics {

struct InstanceLayer {
    std::string name;
    bool required = true;
};

struct InstanceExtension {
    std::string name;
    bool required = true;
};

class Instance : public Handle<vk::Instance> {
public:
    explicit Instance(Window& window);

protected:
    bool createImpl() override;

    void requestLayer(const InstanceLayer& layer);
    void requestExtension(const InstanceExtension& extension);

    void destroyImpl() override;

private:
    Window& m_window;

    vk::DebugUtilsMessengerEXT debugUtilsMessenger = {};

    std::vector<InstanceLayer> m_requestedLayers;
    std::vector<InstanceExtension> m_requestedExtensions;
};

}

#endif
