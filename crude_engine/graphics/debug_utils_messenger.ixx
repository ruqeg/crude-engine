module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.debug_utils_messenger;

import crude.core.std_containers_stack;
import crude.core.shared_ptr;
import crude.graphics.vulkan_object;

export namespace crude::graphics
{

class Instance;

class Debug_Utils_Messenger : public Vulkan_Object<VkDebugUtilsMessengerEXT>
{
public:
  Debug_Utils_Messenger(core::Shared_Ptr<const Instance>      instance,
                        PFN_vkDebugUtilsMessengerCallbackEXT  pfnUserCallback,
                        VkDebugUtilsMessageSeverityFlagsEXT   messageSeverity = DEFAULT_DEBUG_UTILS_MESSENGER_MESSAGE_SEVERITY,
                        VkDebugUtilsMessageTypeFlagsEXT       messageType     = DEFAULT_DEBUG_UTILS_MESSENGER_MESSAGE_TYPE,
                        void*                                 pUserData       = nullptr,
                        const void*                           pNext           = nullptr,
                        VkDebugUtilsMessengerCreateFlagsEXT   flags           = 0u);
  ~Debug_Utils_Messenger();
public:
  static const core::span<const char*> requiredExtensions();
private:
  void destroyDebugUtilsMessengerEXT(VkInstance instance, 
                                     VkDebugUtilsMessengerEXT debugMessenger, 
                                     const VkAllocationCallbacks* pAllocator);
  VkResult createDebugUtilsMessengerEXT(VkInstance instance,
                                        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                        const VkAllocationCallbacks* pAllocator,
                                        VkDebugUtilsMessengerEXT* pDebugMessenger);
private:
  static constexpr VkDebugUtilsMessageSeverityFlagsEXT DEFAULT_DEBUG_UTILS_MESSENGER_MESSAGE_SEVERITY =
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

  static constexpr VkDebugUtilsMessageTypeFlagsEXT DEFAULT_DEBUG_UTILS_MESSENGER_MESSAGE_TYPE =
    VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
private:
  core::Shared_Ptr<const Instance> m_instance;
};

}
