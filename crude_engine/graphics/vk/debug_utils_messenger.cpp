#include <vulkan/vulkan.hpp>

module crude.gfx.vk.debug_utils_messenger;

import crude.gfx.vk.vulkan_utils;
import crude.gfx.vk.instance;
import crude.gfx.vk.extension;
import crude.core.logger;

namespace crude::gfx::vk
{

Debug_Utils_Messenger::Debug_Utils_Messenger(core::shared_ptr<const Instance>      instance,
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

  auto vkCreateDebugUtilsMessengerEXT = getInstanceExtension<PFN_vkCreateDebugUtilsMessengerEXT>(m_instance);
  if (!vkCreateDebugUtilsMessengerEXT)
  {
    core::logError(core::Debug::Channel::Graphics, "failed to create debug utils messenger");
    return;
  }
  const VkResult result = vkCreateDebugUtilsMessengerEXT(m_instance->getHandle(), &vkCreateInfo, getPVkAllocationCallbacks(), &m_handle);
  vulkanHandleResult(result, "failed to create debug utils messenger");
}

Debug_Utils_Messenger::~Debug_Utils_Messenger()
{
  auto vkDestroyDebugUtilsMessengerEXT = getInstanceExtension<PFN_vkDestroyDebugUtilsMessengerEXT>(m_instance);
  
  if (vkDestroyDebugUtilsMessengerEXT)
  {
    vkDestroyDebugUtilsMessengerEXT(m_instance->getHandle(), m_handle, getPVkAllocationCallbacks());
  }
}

const core::span<const char*> Debug_Utils_Messenger::requiredExtensions()
{
  static const char* extensions[] = {VK_EXT_DEBUG_UTILS_EXTENSION_NAME};
  return core::span<const char*>(extensions, 1u);
}

}
