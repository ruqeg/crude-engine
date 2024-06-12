#include <vulkan/vulkan.hpp>

module crude.graphics.instance;

import crude.graphics.vulkan_utils;
import crude.graphics.application;
import crude.graphics.physical_device;

namespace crude::graphics
{
  
Instance::Instance(
#ifdef VK_EXT_debug_utils
                   PFN_vkDebugUtilsMessengerCallbackEXT  debugUtilsCallback,
#endif // VK_EXT_debug_utils
                   const Application&                    application,
                   const core::span<const char* const>&  enabledExtensions,
                   const core::span<const char* const>&  enabledLayers,
                   VkInstanceCreateFlags                 flags)
{
  VkInstanceCreateInfo vkInstanceCreateInfo{};
  vkInstanceCreateInfo.sType                    = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  vkInstanceCreateInfo.pApplicationInfo         = &application;
  vkInstanceCreateInfo.flags                    = flags;
  vkInstanceCreateInfo.ppEnabledExtensionNames  = enabledExtensions.data();
  vkInstanceCreateInfo.enabledExtensionCount    = enabledExtensions.size();

  const core::uint32 ebabledLayersCount = enabledLayers.size();

#ifdef VK_EXT_debug_utils
  VkDebugUtilsMessengerCreateInfoEXT vkDebugCreateInfo{};
#endif // VK_EXT_debug_utils

  if (ebabledLayersCount > 0u)
  {
    vkInstanceCreateInfo.ppEnabledLayerNames     = enabledLayers.data();
    vkInstanceCreateInfo.enabledLayerCount       = enabledLayers.size();

#ifdef VK_EXT_debug_utils
    void* pDebugNext      = nullptr;
    void* pDebugUserData  = nullptr;
    core::uint32 debugFlags     = 0u;

    vkDebugCreateInfo.sType            = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    vkDebugCreateInfo.pNext            = pDebugNext;
    vkDebugCreateInfo.flags            = debugFlags;
    vkDebugCreateInfo.pfnUserCallback  = debugUtilsCallback;
    vkDebugCreateInfo.pUserData        = pDebugUserData;

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
  
  const VkResult result = vkCreateInstance(&vkInstanceCreateInfo, getPVkAllocationCallbacks(), &m_handle);
  vulkanHandleResult(result, "failed to create instance");
}
 
Instance::~Instance()
{
  vkDestroyInstance(m_handle, getPVkAllocationCallbacks());
}
  
core::vector<core::shared_ptr<Physical_Device>> Instance::getPhysicalDevices()
{
  core::uint32 vkPhysicalDeviceCount = 0u;
  vkEnumeratePhysicalDevices(
    m_handle, 
    &vkPhysicalDeviceCount, 
    nullptr);
  
  if (vkPhysicalDeviceCount == 0u) 
  {
    return {};
  }

  core::vector<VkPhysicalDevice> vkPhysicalDevices(vkPhysicalDeviceCount);
  vkEnumeratePhysicalDevices(
      m_handle, 
      &vkPhysicalDeviceCount,
      vkPhysicalDevices.data());

  core::vector<core::shared_ptr<Physical_Device>> physicaDevices(vkPhysicalDeviceCount);
  for (core::uint32 i = 0u; i < vkPhysicalDeviceCount; ++i)
  {
    physicaDevices[i] = core::allocateShared<Physical_Device>(vkPhysicalDevices[i]);
  }
  
  return physicaDevices;
}

}
