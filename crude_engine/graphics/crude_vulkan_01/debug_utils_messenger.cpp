#include "debug_utils_messenger.hpp"
#include "instance.hpp"

namespace crude_vulkan_01
{
  
DebugUtilsMessengerCreateInfo::DebugUtilsMessengerCreateInfo(std::shared_ptr<const Instance>       instance,
                                                             PFN_vkDebugUtilsMessengerCallbackEXT  pfnUserCallback,
                                                             VkDebugUtilsMessageSeverityFlagsEXT   messageSeverity,
                                                             VkDebugUtilsMessageTypeFlagsEXT       messageType,
                                                             void*                                 pUserData,
                                                             const void*                           pNext,
                                                             VkDebugUtilsMessengerCreateFlagsEXT   flags)
  :
    instance(instance),
    pfnUserCallback(pfnUserCallback),
    messageSeverity(messageSeverity),
    messageType(messageType),
    pUserData(pUserData),
    pNext(pNext),
    flags(flags)
{}

VkResult Debug_Utils_Messenger::createDebugUtilsMessengerEXT(VkInstance instance,
                                                             const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                                             const VkAllocationCallbacks* pAllocator,
                                                             VkDebugUtilsMessengerEXT* pDebugMessenger) {
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr) 
  {
    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
  } else
  {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void Debug_Utils_Messenger::destroyDebugUtilsMessengerEXT(VkInstance instance, 
                                                          VkDebugUtilsMessengerEXT debugMessenger, 
                                                          const VkAllocationCallbacks* pAllocator) {
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
  if (func != nullptr) 
  {
    func(instance, debugMessenger, pAllocator);
  }
}

Debug_Utils_Messenger::Debug_Utils_Messenger(const DebugUtilsMessengerCreateInfo& createInfo)
{
  VkDebugUtilsMessengerCreateInfoEXT vkCreateInfo{};
  vkCreateInfo.sType            = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  vkCreateInfo.pfnUserCallback  = createInfo.pfnUserCallback;
  vkCreateInfo.messageSeverity  = createInfo.messageSeverity;
  vkCreateInfo.messageType      = createInfo.messageType;
  vkCreateInfo.pUserData        = createInfo.pUserData;
  vkCreateInfo.pNext            = createInfo.pNext;
  vkCreateInfo.flags            = createInfo.flags;

  m_instance = createInfo.instance;

  const VkAllocationCallbacks* pAllocator = nullptr;
  const VkResult result = createDebugUtilsMessengerEXT(CRUDE_VULKAN_01_HANDLE(m_instance), &vkCreateInfo, pAllocator, &m_handle);
  CRUDE_VULKAN_01_HANDLE_RESULT(result, "failed to create debug utils messenger");
}

Debug_Utils_Messenger::~Debug_Utils_Messenger()
{
  destroyDebugUtilsMessengerEXT(CRUDE_VULKAN_01_HANDLE(m_instance), m_handle, nullptr);
}
  
const std::vector<const char*>& Debug_Utils_Messenger::requiredExtensions()
{
  static std::vector<const char*> extensions = {VK_EXT_DEBUG_UTILS_EXTENSION_NAME};
  return extensions;
}

}
