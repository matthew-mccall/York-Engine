#include <sstream>
#include <vector>

#include <SDL2/SDL_vulkan.h>

#include "york/Graphics/Instance.hpp"
#include "york/Log.hpp"

namespace {

std::vector<vk::LayerProperties> s_availableLayers;
std::vector<vk::ExtensionProperties> s_availableExtensions;
VKAPI_ATTR VkBool32 VKAPI_CALL debugMessageFunc(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, VkDebugUtilsMessengerCallbackDataEXT const* pCallbackData, void* /*pUserData*/)
{

    switch (messageSeverity) {

    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
        YORK_CORE_TRACE(pCallbackData->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
        YORK_CORE_INFO(pCallbackData->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        YORK_CORE_WARN(pCallbackData->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        YORK_CORE_ERROR(pCallbackData->pMessage);
        break;
    default:
        YORK_CORE_DEBUG(pCallbackData->pMessage);
        break;
    }

    return false;
}

}

namespace york::graphics {

bool Instance::createImpl()
{
    vk::ApplicationInfo appInfo { "York Engine Client", VK_MAKE_VERSION(1, 0, 0), "York Engine", VK_MAKE_VERSION(1, 0, 0), VK_API_VERSION_1_1 };

    std::vector<const char*> enabledLayers;
    std::vector<const char*> enabledExtensions;
    std::vector<InstanceLayer> unavailableLayers;
    std::vector<InstanceExtension> unavailableExtensions;

#ifndef NDEBUG
    requestLayer({ "VK_LAYER_KHRONOS_validation" });
    requestExtension({ VK_EXT_DEBUG_UTILS_EXTENSION_NAME });
#endif

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

    std::vector<const char*> sdlExtensions;

    {
        // TODO: Remove when SDL updates their API
        SDL_Window* window = SDL_CreateWindow("York Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
        unsigned extensionCount;

        if (!SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, nullptr)) {
            log::core::error(SDL_GetError());
            return false;
        }

        sdlExtensions.resize(extensionCount);

        if (!SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, sdlExtensions.data())) {
            log::core::error(SDL_GetError());
            return false;
        }

        SDL_DestroyWindow(window);
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

    vk::InstanceCreateInfo createInfo { {}, &appInfo, enabledLayers, enabledExtensions };

    vk::DebugUtilsMessengerCreateInfoEXT messengerCreateInfo = vk::DebugUtilsMessengerCreateInfoEXT()
                                                                   .setMessageSeverity(/* vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | */ vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError)
                                                                   .setMessageType(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance)
                                                                   .setPfnUserCallback(debugMessageFunc);

#ifndef NDEBUG
    createInfo.setPNext(reinterpret_cast<VkDebugUtilsMessengerCreateInfoEXT*>(&messengerCreateInfo));
#endif

    m_handle = vk::createInstance(createInfo);

#ifndef NDEBUG
    debugUtilsMessenger = m_handle.createDebugUtilsMessengerEXT(messengerCreateInfo, nullptr, vk::DispatchLoaderDynamic(static_cast<VkInstance>(m_handle), vkGetInstanceProcAddr));
#endif

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
#ifndef NDEBUG
    m_handle.destroy(debugUtilsMessenger, nullptr, vk::DispatchLoaderDynamic(static_cast<VkInstance>(m_handle), vkGetInstanceProcAddr));
#endif

    m_handle.destroy();
}

} // namespace york::graphics
