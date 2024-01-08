#include <graphics/vulkan/device.hpp>
#include <graphics/vulkan/fence.hpp>

namespace crude_engine
{

Fence::Fence(Shared_Ptr<const Device> device, VkFenceCreateFlags flags)
  :
  m_device(device)
{
  VkFenceCreateInfo vkCreateInfo{};
  vkCreateInfo.sType  = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  vkCreateInfo.pNext  = nullptr;
  vkCreateInfo.flags  = flags;

  VkResult result = vkCreateFence(CRUDE_OBJECT_HANDLE(m_device), &vkCreateInfo, &getVkAllocationCallbacks(), &m_handle);
  CRUDE_VULKAN_HANDLE_RESULT(result, "failed to create fence");
}

Fence::~Fence()
{
  vkDestroyFence(CRUDE_OBJECT_HANDLE(m_device), m_handle, &getVkAllocationCallbacks());
}

bool Fence::wait(uint64 timeout)
{
  constexpr bool waitAllFence = true;
  const VkResult result = vkWaitForFences(CRUDE_OBJECT_HANDLE(m_device), 1, &m_handle, waitAllFence, timeout);
  CRUDE_VULKAN_HANDLE_RESULT(result, "failed to wait for fence");
  return result != VK_TIMEOUT;
}

bool Fence::reset()
{
  const VkResult result = vkResetFences(CRUDE_OBJECT_HANDLE(m_device), 1u, &m_handle);
  CRUDE_VULKAN_HANDLE_RESULT(result, "failed to reset fence");
  return result != VK_ERROR_OUT_OF_DEVICE_MEMORY;
}

}
