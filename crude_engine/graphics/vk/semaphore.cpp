#include <vulkan/vulkan.hpp>

module crude.gfx.vk.semaphore;

import crude.gfx.vk.device;
import crude.gfx.vk.vulkan_utils;

namespace crude::gfx::vk
{

Semaphore::Semaphore(core::shared_ptr<const Device> device, VkSemaphoreCreateFlags flags)
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
