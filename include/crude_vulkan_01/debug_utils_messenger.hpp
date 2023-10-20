#pragma once

#include "core.hpp"
#include "include_vulkan.hpp"
#include "object.hpp"
#include "requiring_extensions.hpp"

namespace crude_vulkan_01
{

class Instance;

struct DebugUtilsMessengerCreateInfo
{
  static constexpr VkDebugUtilsMessageSeverityFlagsEXT DEFAULT_DEBUG_UTILS_MESSENGER_MESSAGE_SEVERITY = 
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  
  static constexpr VkDebugUtilsMessageTypeFlagsEXT DEFAULT_DEBUG_UTILS_MESSENGER_MESSAGE_TYPE =
    VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

  std::shared_ptr<const Instance>       instance;
  const void*                           pNext;
  VkDebugUtilsMessengerCreateFlagsEXT   flags;
  VkDebugUtilsMessageSeverityFlagsEXT   messageSeverity;
  VkDebugUtilsMessageTypeFlagsEXT       messageType;
  PFN_vkDebugUtilsMessengerCallbackEXT  pfnUserCallback;
  void*                                 pUserData;

  explicit DebugUtilsMessengerCreateInfo(std::shared_ptr<const Instance>       instance,
                                         PFN_vkDebugUtilsMessengerCallbackEXT  pfnUserCallback,
                                         VkDebugUtilsMessageSeverityFlagsEXT   messageSeverity = DEFAULT_DEBUG_UTILS_MESSENGER_MESSAGE_SEVERITY,
                                         VkDebugUtilsMessageTypeFlagsEXT       messageType     = DEFAULT_DEBUG_UTILS_MESSENGER_MESSAGE_TYPE,
                                         void*                                 pUserData       = nullptr,
                                         const void*                           pNext           = nullptr,
                                         VkDebugUtilsMessengerCreateFlagsEXT   flags           = 0u);
};

class Debug_Utils_Messenger : public TObject<VkDebugUtilsMessengerEXT>, public Requiring_Extensions
{
public:
  Debug_Utils_Messenger(const DebugUtilsMessengerCreateInfo& createInfo);
  ~Debug_Utils_Messenger();
  static const std::vector<const char*>& requiredExtensions();
private:
  void destroyDebugUtilsMessengerEXT(VkInstance instance, 
                                     VkDebugUtilsMessengerEXT debugMessenger, 
                                     const VkAllocationCallbacks* pAllocator);
  VkResult createDebugUtilsMessengerEXT(VkInstance instance,
                                        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                        const VkAllocationCallbacks* pAllocator,
                                        VkDebugUtilsMessengerEXT* pDebugMessenger);
private:
  std::shared_ptr<const Instance> m_instance;
};

}
