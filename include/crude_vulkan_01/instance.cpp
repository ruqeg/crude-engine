#include "instance.hpp"
#include "application.hpp"
#include <stdexcept>
#include <vulkan/vulkan_core.h>

namespace crude_vulkan_01
{
  
InstanceCreateInfo::InstanceCreateInfo(const Application&              application,
#ifdef VK_EXT_debug_utils
                                       PFN_vkDebugUtilsMessengerCallbackEXT debugUtilsCallback,
#endif // VK_EXT_debug_utils
                                       VkInstanceCreateFlags           flags,
                                       const std::vector<const char*>& enabledExtensions,
                                       const std::vector<const char*>& enabledLayers)
  :
    application(application),
    flags(flags),
    enabledExtensions(enabledExtensions),
    enabledLayers(enabledLayers)
{}
  
Instance::Instance(const InstanceCreateInfo& createInfo)
{
  VkInstanceCreateInfo vkInstanceCreateInfo{};
  vkInstanceCreateInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  vkInstanceCreateInfo.pApplicationInfo        = &createInfo.application;
  vkInstanceCreateInfo.flags                   = createInfo.flags;
  vkInstanceCreateInfo.ppEnabledExtensionNames = createInfo.enabledExtensions.data();
  vkInstanceCreateInfo.enabledExtensionCount   = createInfo.enabledExtensions.size();

  const uint32 ebabledLayersCount = createInfo.enabledLayers.size();

#ifdef VK_EXT_debug_utils
    VkDebugUtilsMessengerCreateInfoEXT vkDebugCreateInfo{};
#endif // VK_EXT_debug_utils

  if (ebabledLayersCount > 0u)
  {
    vkInstanceCreateInfo.ppEnabledLayerNames     = createInfo.enabledLayers.data();
    vkInstanceCreateInfo.enabledLayerCount       = createInfo.enabledLayers.size();

#ifdef VK_EXT_debug_utils
    void* pDebugNext      = nullptr;
    void* pDebugUserData  = nullptr;
    uint32 debugFlags    = 0u;

    vkDebugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    vkDebugCreateInfo.pNext           = pDebugNext;
    vkDebugCreateInfo.flags           = debugFlags;
    vkDebugCreateInfo.pfnUserCallback = createInfo.debugUtilsCallback;
    vkDebugCreateInfo.pUserData       = pDebugUserData;

    vkDebugCreateInfo.messageSeverity =
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    vkDebugCreateInfo.messageType =
      VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        
    vkInstanceCreateInfo.pNext = &vkDebugCreateInfo;
#else
    vkInstanceCreateInfo.pNext = nullptr;
#endif // VK_EXT_debug_utils
  }
  else
  {
    vkInstanceCreateInfo.ppEnabledLayerNames = nullptr;
    vkInstanceCreateInfo.enabledLayerCount = 0u;
  }
  
  VkAllocationCallbacks* pAllocator = nullptr;
  const VkResult result = vkCreateInstance(&vkInstanceCreateInfo, pAllocator, &m_handle);
  
  if (result != VK_SUCCESS) 
  {
#ifndef CRUDE_VULKAN_01_NO_EXCEPTIONS
    throw std::runtime_error("failed to create instance");
#endif //CRUDE_VULKAN_01_NO_EXCEPTIONS
  }
  
  CRUDE_VULKAN_01_HANDLE_RESULT(result, "failed to create instance");
}
 
Instance::~Instance()
{
  VkAllocationCallbacks* pAllocator = nullptr;
  vkDestroyInstance(m_handle, pAllocator);
}

}
