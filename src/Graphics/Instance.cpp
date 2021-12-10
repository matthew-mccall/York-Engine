#include <sstream>
#include <vector>

#include <SDL_vulkan.h>

#include "york/Graphics/Instance.hpp"
#include "york/Log.hpp"

#ifdef NDEBUG
constexpr static bool ENABLE_VALIDATIONS_LAYERS = false;
#else
constexpr static bool ENABLE_VALIDATIONS_LAYERS = true;
#endif

namespace {

std::vector<vk::LayerProperties> s_availableLayers;
std::vector<vk::ExtensionProperties> s_availableExtensions;
VKAPI_ATTR VkBool32 VKAPI_CALL debugMessageFunc(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, VkDebugUtilsMessengerCallbackDataEXT const* pCallbackData, void* /*pUserData*/)
{

    switch (messageSeverity) {

    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
        york::log::core::trace(pCallbackData->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
        york::log::core::info(pCallbackData->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        york::log::core::warn(pCallbackData->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        york::log::core::error(pCallbackData->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
        break;
    }

    return false;
}

}

namespace york::graphics {

Instance::Instance(Window& window)
    : m_window(window)
{
}

bool Instance::createImpl()
{
    vk::ApplicationInfo appInfo = { m_window.getName().c_str(), VK_MAKE_VERSION(1, 0, 0), "York Engine", VK_MAKE_VERSION(1, 0, 0), VK_API_VERSION_1_1 };

    std::vector<const char*> enabledLayers;
    std::vector<const char*> enabledExtensions;
    std::vector<InstanceLayer> unavailableLayers;
    std::vector<InstanceExtension> unavailableExtensions;

    if (ENABLE_VALIDATIONS_LAYERS) {
        requestLayer({ "VK_LAYER_KHRONOS_validation" });
        requestExtension({ VK_EXT_DEBUG_UTILS_EXTENSION_NAME });
    }

    if (s_availableLayers.empty()) {
        s_availableLayers = vk::enumerateInstanceLayerProperties();
    }

    for (InstanceLayer& requestedLayer : m_requestedLayers) {

        bool layerFound = false;

        for (vk::LayerProperties& availableLayer : s_availableLayers) {
            if (requestedLayer.name == availableLayer.layerName) {
                enabledLayers.push_back(availableLayer.layerName);
                layerFound = true;
            }
        }

        if (layerFound)
            continue;

        unavailableLayers.push_back(requestedLayer);
    }

    if (s_availableExtensions.empty()) {
        s_availableExtensions = vk::enumerateInstanceExtensionProperties();
    }

    unsigned extensionCount;

    if (!SDL_Vulkan_GetInstanceExtensions(m_window.getHandle(), &extensionCount, nullptr)) {
        log::core::error(SDL_GetError());
        return false;
    }

    std::vector<const char*> sdlExtensions(extensionCount);

    if (!SDL_Vulkan_GetInstanceExtensions(m_window.getHandle(), &extensionCount, sdlExtensions.data())) {
        log::core::error(SDL_GetError());
        return false;
    }

    for (const char* sdlExtension : sdlExtensions) {
        requestExtension({ sdlExtension });
    }

    for (InstanceExtension& requestedExtension : m_requestedExtensions) {

        bool extensionFound = false;

        for (vk::ExtensionProperties& availableExtension : s_availableExtensions) {
            if (requestedExtension.name == availableExtension.extensionName) {
                enabledExtensions.push_back(availableExtension.extensionName);
                extensionFound = true;
            }
        }

        if (extensionFound)
            continue;

        unavailableExtensions.push_back(requestedExtension);
    }

    if (!unavailableLayers.empty()) {
        std::stringstream unavailableRequiredLayersList;
        std::stringstream unavailableOptionalLayersList;

        for (InstanceLayer& unavailableLayer : unavailableLayers) {
            if (unavailableLayer.required) {
                unavailableRequiredLayersList << '\n'
                                              << unavailableLayer.name;
                continue;
            }

            unavailableOptionalLayersList << '\n'
                                          << unavailableLayer.name;
        }

        log::core::error("The following REQUIRED instance layers were requested but not found!{}", unavailableRequiredLayersList.str());
        log::core::warn("The following OPTIONAL instance layers were requested but not found!{}", unavailableOptionalLayersList.str());
    }

    if (!unavailableExtensions.empty()) {
        std::stringstream unavailableRequiredExtensionList;
        std::stringstream unavailableOptionalExtensionList;

        for (InstanceExtension& unavailableExtension : unavailableExtensions) {
            if (unavailableExtension.required) {
                unavailableRequiredExtensionList << '\n'
                                                 << unavailableExtension.name;
                continue;
            }

            unavailableOptionalExtensionList << '\n'
                                             << unavailableExtension.name;
        }

        log::core::error("The following REQUIRED instance extensions were requested but not found!\n{}", unavailableRequiredExtensionList.str());
        log::core::warn("The following OPTIONAL instance extensions were requested but not found!\n{}", unavailableOptionalExtensionList.str());
    }

    vk::InstanceCreateInfo createInfo({}, &appInfo, enabledLayers, enabledExtensions);

    vk::DebugUtilsMessengerCreateInfoEXT messengerCreateInfo = vk::DebugUtilsMessengerCreateInfoEXT()
                                                                   .setMessageSeverity(vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError)
                                                                   .setMessageType(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance)
                                                                   .setPfnUserCallback(debugMessageFunc);

    if (ENABLE_VALIDATIONS_LAYERS) {
        createInfo.setPNext(reinterpret_cast<VkDebugUtilsMessengerCreateInfoEXT*>(&messengerCreateInfo));
    }

    m_handle = vk::createInstance(createInfo);

    if (ENABLE_VALIDATIONS_LAYERS) {
        debugUtilsMessenger = m_handle.createDebugUtilsMessengerEXT(messengerCreateInfo, nullptr, vk::DispatchLoaderDynamic(static_cast<VkInstance>(m_handle), vkGetInstanceProcAddr));
    }

    return true;
}

void Instance::requestLayer(const InstanceLayer& layer)
{
    m_requestedLayers.push_back(layer);
}

void Instance::requestExtension(const InstanceExtension& extension)
{
    m_requestedExtensions.push_back(extension);
}

void Instance::destroyImpl()
{
    if (ENABLE_VALIDATIONS_LAYERS) {
        m_handle.destroy(debugUtilsMessenger, nullptr, vk::DispatchLoaderDynamic(static_cast<VkInstance>(m_handle), vkGetInstanceProcAddr));
    }

    m_handle.destroy();
}

} // namespace york::graphics
