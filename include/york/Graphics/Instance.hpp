#if !defined(YORK_GRAPHICS_INSTANCE_HPP)
#define YORK_GRAPHICS_INSTANCE_HPP

#include <string>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "Handle.hpp"
#include "RequestableItem.hpp"
#include "york/Window.hpp"

/**
 * The namespace for everything Vulkan related. Engine only. (Except Window)
 */
namespace york::graphics {

using InstanceLayer = RequestableItem;
using InstanceExtension = RequestableItem;

/**
 * A handle for Vulkan instances
 */
class Instance : public Handle<vk::Instance> {
public:
    /**
     * Creates a Vulkan instance.
     *
     * @param window The window which we will be drawing to.
     */
    explicit Instance() = default;

protected:
    bool createImpl() override;

    /**
     * @brief Requests an instance layer to be used in the creation of the instance.
     *
     * If supported, the instance will be created with the layer.
     *
     * @param layer The layer to be requested and whether it is required.
     */
    void requestLayer(const InstanceLayer& layer);

    /**
     * @brief Requests an instance extension to be used in the creation of the instance.
     *
     * If supported, the instance will be created with the extension.
     *
     * @param extension The extension to be requested and whether it is required.
     */
    void requestExtension(const InstanceExtension& extension);

    void destroyImpl() override;

private:
    vk::DebugUtilsMessengerEXT debugUtilsMessenger = {};
    std::vector<InstanceLayer> m_requestedLayers;
    std::vector<InstanceExtension> m_requestedExtensions;
};

}

#endif
