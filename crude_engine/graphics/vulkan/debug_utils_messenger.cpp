#include <graphics/vulkan/debug_utils_messenger.hpp>
#include <graphics/vulkan/instance.hpp>

namespace crude_engine
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

Debug_Utils_Messenger::Debug_Utils_Messenger(Shared_Ptr<const Instance>            instance,
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

  const VkResult result = createDebugUtilsMessengerEXT(CRUDE_OBJECT_HANDLE(m_instance), &vkCreateInfo, getPVkAllocationCallbacks(), &m_handle);
  CRUDE_VULKAN_HANDLE_RESULT(result, "failed to create debug utils messenger");
}

Debug_Utils_Messenger::~Debug_Utils_Messenger()
{
  destroyDebugUtilsMessengerEXT(CRUDE_OBJECT_HANDLE(m_instance), m_handle, getPVkAllocationCallbacks());
}

const span<const char*> Debug_Utils_Messenger::requiredExtensions()
{
  static const char* extensions[] = {VK_EXT_DEBUG_UTILS_EXTENSION_NAME};
  return span<const char*>(extensions, 1u);
}

}
