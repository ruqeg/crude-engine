#include <graphics/vulkan/instance.hpp>
#include <graphics/vulkan/application.hpp>
#include <graphics/vulkan/physical_device.hpp>

namespace crude_engine
{
  
Instance::Instance(
#ifdef VK_EXT_debug_utils
                   PFN_vkDebugUtilsMessengerCallbackEXT  debugUtilsCallback,
#endif // VK_EXT_debug_utils
                   const Application&                    application,
                   const Array_Unsafe<const char*>&      enabledExtensions,
                   const Array_Unsafe<const char*>&      enabledLayers,
                   VkInstanceCreateFlags                 flags)
{
  VkInstanceCreateInfo vkInstanceCreateInfo{};
  vkInstanceCreateInfo.sType                    = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  vkInstanceCreateInfo.pApplicationInfo         = &application;
  vkInstanceCreateInfo.flags                    = flags;
  vkInstanceCreateInfo.ppEnabledExtensionNames  = enabledExtensions.data();
  vkInstanceCreateInfo.enabledExtensionCount    = enabledExtensions.size();

  const uint32 ebabledLayersCount = enabledLayers.size();

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
    uint32 debugFlags     = 0u;

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
  
  const VkResult result = vkCreateInstance(&vkInstanceCreateInfo, &getVkAllocationCallbacks(), &m_handle);
  CRUDE_VULKAN_HANDLE_RESULT(result, "failed to create instance");
}
 
Instance::~Instance()
{
  vkDestroyInstance(m_handle, &getVkAllocationCallbacks());
}
  
Array_Dynamic<Shared_Ptr<Physical_Device>> Instance::getPhysicalDevices()
{
  uint32 vkPhysicalDeviceCount = 0u;
  vkEnumeratePhysicalDevices(
    m_handle, 
    &vkPhysicalDeviceCount, 
    nullptr);
  
  if (vkPhysicalDeviceCount == 0u) 
  {
    return {};
  }

  Array_Dynamic<VkPhysicalDevice> vkPhysicalDevices(vkPhysicalDeviceCount);
  vkEnumeratePhysicalDevices(
      m_handle, 
      &vkPhysicalDeviceCount,
      vkPhysicalDevices.data());

  Array_Dynamic<Shared_Ptr<Physical_Device>> physicaDevices(vkPhysicalDeviceCount);
  for (uint32 i = 0u; i < vkPhysicalDeviceCount; ++i) 
  {
    physicaDevices[i] = makeShared<Physical_Device>(vkPhysicalDevices[i]);
  }
  
  return physicaDevices;
}

}
