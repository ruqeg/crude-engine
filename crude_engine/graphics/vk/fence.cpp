#include <vulkan/vulkan.hpp>

module crude.gfx.vk.fence;

import crude.gfx.vk.device;
import crude.gfx.vk.vulkan_utils;

namespace crude::gfx::vk
{

Fence::Fence(core::shared_ptr<const Device> device, VkFenceCreateFlags flags)
  :
  m_device(device)
{
  VkFenceCreateInfo vkCreateInfo{};
  vkCreateInfo.sType  = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  vkCreateInfo.pNext  = nullptr;
  vkCreateInfo.flags  = flags;

  VkResult result = vkCreateFence(m_device->getHandle(), &vkCreateInfo, getPVkAllocationCallbacks(), &m_handle);
  vulkanHandleResult(result, "failed to create fence");
}

Fence::~Fence()
{
  vkDestroyFence(m_device->getHandle(), m_handle, getPVkAllocationCallbacks());
}

bool Fence::wait(core::uint64 timeout)
{
  constexpr bool waitAllFence = true;
  const VkResult result = vkWaitForFences(m_device->getHandle(), 1, &m_handle, waitAllFence, timeout);
  vulkanHandleResult(result, "failed to wait for fence");
  return result != VK_TIMEOUT;
}

bool Fence::reset()
{
  const VkResult result = vkResetFences(m_device->getHandle(), 1u, &m_handle);
  vulkanHandleResult(result, "failed to reset fence");
  return result != VK_ERROR_OUT_OF_DEVICE_MEMORY;
}

}
