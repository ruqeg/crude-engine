#include <vulkan/vulkan.hpp>

module crude_engine.graphics.vulkan.semaphore;

import crude_engine.graphics.vulkan.device;
import crude_engine.graphics.vulkan.vulkan_utils;

namespace crude_engine
{

Semaphore::Semaphore(Shared_Ptr<const Device> device, VkSemaphoreCreateFlags flags)
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
