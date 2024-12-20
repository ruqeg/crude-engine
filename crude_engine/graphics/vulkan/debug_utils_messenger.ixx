module;

#include <vulkan/vulkan.hpp>

export module crude.gfx.vk.debug_utils_messenger;

import crude.core.std_containers_stack;
import crude.core.std_containers_heap;
import crude.gfx.vk.vulkan_object;

namespace crude::gfx::vk
{

static constexpr VkDebugUtilsMessageSeverityFlagsEXT cDefaultDebugUtilsMessengerMessageSeverity =
VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

static constexpr VkDebugUtilsMessageTypeFlagsEXT cDefaultDebugUtilsMessengerMessageType =
VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

}

export namespace crude::gfx::vk
{

class Instance;

class Debug_Utils_Messenger : public Vulkan_Object<VkDebugUtilsMessengerEXT>
{
public:
  Debug_Utils_Messenger(core::shared_ptr<const Instance>      instance,
                        PFN_vkDebugUtilsMessengerCallbackEXT  pfnUserCallback,
                        VkDebugUtilsMessageSeverityFlagsEXT   messageSeverity = cDefaultDebugUtilsMessengerMessageSeverity,
                        VkDebugUtilsMessageTypeFlagsEXT       messageType     = cDefaultDebugUtilsMessengerMessageType,
                        void*                                 pUserData       = nullptr,
                        const void*                           pNext           = nullptr,
                        VkDebugUtilsMessengerCreateFlagsEXT   flags           = 0u);
  ~Debug_Utils_Messenger();
public:
  static const core::span<const char*> requiredExtensions();
private:
  core::shared_ptr<const Instance> m_instance;
};

}
