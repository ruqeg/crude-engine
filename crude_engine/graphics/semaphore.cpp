#include <vulkan/vulkan.hpp>

module crude.graphics.semaphore;

import crude.graphics.device;
import crude.graphics.vulkan_utils;

namespace crude::graphics
{

Semaphore::Semaphore(core::Shared_Ptr<const Device> device, VkSemaphoreCreateFlags flags)
  :
  m_device(device)
{
  VkSemaphoreCreateInfo vkCreateInfo{};
  vkCreateInfo.sType  = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
  vkCreateInfo.pNext  = nullptr;
  vkCreateInfo.flags  = flags;

  VkResult result = vkCreateSemaphore(m_device->getHandle(), &vkCreateInfo, getPVkAllocationCallbacks(), &m_handle);
  vulkanHandleResult(result, "failed to create semaphore");
}
 
Semaphore::~Semaphore()
{
  vkDestroySemaphore(m_device->getHandle(), m_handle, getPVkAllocationCallbacks());
}

}
