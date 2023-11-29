#include "instance.hpp"
#include "application.hpp"
#include "physical_device.hpp"

namespace crude_vulkan_01
{
  
Instance_Create_Info::Instance_Create_Info(
#ifdef VK_EXT_debug_utils
                                       PFN_vkDebugUtilsMessengerCallbackEXT debugUtilsCallback,
#endif // VK_EXT_debug_utils
                                       const Application&              application,
                                       const std::vector<const char*>& enabledExtensions,
                                       const std::vector<const char*>& enabledLayers,
                                       VkInstanceCreateFlags           flags)
  :
#ifdef VK_EXT_debug_utils
    debugUtilsCallback(debugUtilsCallback),
#endif // VK_EXT_debug_utils
    application(application),
    enabledExtensions(enabledExtensions),
    enabledLayers(enabledLayers),
    flags(flags)
{}
  
Instance::Instance(const Instance_Create_Info& createInfo)
{
  VkInstanceCreateInfo vkInstanceCreateInfo{};
  vkInstanceCreateInfo.sType                    = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  vkInstanceCreateInfo.pApplicationInfo         = &createInfo.application;
  vkInstanceCreateInfo.flags                    = createInfo.flags;
  vkInstanceCreateInfo.ppEnabledExtensionNames  = createInfo.enabledExtensions.data();
  vkInstanceCreateInfo.enabledExtensionCount    = createInfo.enabledExtensions.size();

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
    uint32 debugFlags     = 0u;

    vkDebugCreateInfo.sType            = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    vkDebugCreateInfo.pNext            = pDebugNext;
    vkDebugCreateInfo.flags            = debugFlags;
    vkDebugCreateInfo.pfnUserCallback  = createInfo.debugUtilsCallback;
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
  
std::vector<std::shared_ptr<Physical_Device>> Instance::getPhysicalDevices()
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

  std::vector<VkPhysicalDevice> vkPhysicalDevices(vkPhysicalDeviceCount);
  vkEnumeratePhysicalDevices(
      m_handle, 
      &vkPhysicalDeviceCount,
      vkPhysicalDevices.data());

  std::vector<std::shared_ptr<Physical_Device>> physicaDevices(vkPhysicalDeviceCount);
  for (uint32 i = 0u; i < vkPhysicalDeviceCount; ++i) 
  {
    physicaDevices[i] = std::make_shared<Physical_Device>(Physical_Device_Create_Info(vkPhysicalDevices[i]));
  }
  
  return physicaDevices;
}

}
