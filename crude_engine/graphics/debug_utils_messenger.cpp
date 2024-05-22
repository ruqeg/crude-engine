#include <vulkan/vulkan.hpp>

module crude.graphics.vulkan.debug_utils_messenger;

import crude.graphics.vulkan.vulkan_utils;
import crude.graphics.vulkan.instance;

namespace crude::graphics
{

VkResult Debug_Utils_Messenger::createDebugUtilsMessengerEXT(VkInstance                                 instance,
                                                             const VkDebugUtilsMessengerCreateInfoEXT*  pCreateInfo,
                                                             const VkAllocationCallbacks*               pAllocator,
                                                             VkDebugUtilsMessengerEXT*                  pDebugMessenger) {
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr) 
  {
    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
  } else
  {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void Debug_Utils_Messenger::destroyDebugUtilsMessengerEXT(VkInstance                    instance, 
                                                          VkDebugUtilsMessengerEXT      debugMessenger, 
                                                          const VkAllocationCallbacks*  pAllocator) {
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
  if (func != nullptr) 
  {
    func(instance, debugMessenger, pAllocator);
  }
}

Debug_Utils_Messenger::Debug_Utils_Messenger(core::Shared_Ptr<const Instance>      instance,
                                             PFN_vkDebugUtilsMessengerCallbackEXT  pfnUserCallback,
                                             VkDebugUtilsMessageSeverityFlagsEXT   messageSeverity,
                                             VkDebugUtilsMessageTypeFlagsEXT       messageType,
                                             void*                                 pUserData,
                                             const void*                           pNext,
                                             VkDebugUtilsMessengerCreateFlagsEXT   flags)
  :
  m_instance(instance)
{
  VkDebugUtilsMessengerCreateInfoEXT vkCreateInfo{};
  vkCreateInfo.sType            = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  vkCreateInfo.pfnUserCallback  = pfnUserCallback;
  vkCreateInfo.messageSeverity  = messageSeverity;
  vkCreateInfo.messageType      = messageType;
  vkCreateInfo.pUserData        = pUserData;
  vkCreateInfo.pNext            = pNext;
  vkCreateInfo.flags            = flags;

  const VkResult result = createDebugUtilsMessengerEXT(m_instance->getHandle(), &vkCreateInfo, getPVkAllocationCallbacks(), &m_handle);
  vulkanHandleResult(result, "failed to create debug utils messenger");
}

Debug_Utils_Messenger::~Debug_Utils_Messenger()
{
  destroyDebugUtilsMessengerEXT(m_instance->getHandle(), m_handle, getPVkAllocationCallbacks());
}

const core::span<const char*> Debug_Utils_Messenger::requiredExtensions()
{
  static const char* extensions[] = {VK_EXT_DEBUG_UTILS_EXTENSION_NAME};
  return core::span<const char*>(extensions, 1u);
}

}
