#include "fence.hpp"
#include "device.hpp"

namespace crude_vulkan_01
{

Fence_Create_Info::Fence_Create_Info(std::shared_ptr<const Device> device, VkFenceCreateFlags flags)
  :
  device(device),
  flags(flags)
{}

Fence::Fence(const Fence_Create_Info& createInfo)
{
  m_device = createInfo.device;

  VkFenceCreateInfo vkCreateInfo{};
  vkCreateInfo.sType  = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  vkCreateInfo.pNext  = nullptr;
  vkCreateInfo.flags  = createInfo.flags;

  VkResult result = vkCreateFence(CRUDE_VULKAN_01_HANDLE(m_device), &vkCreateInfo, nullptr, &m_handle);
  CRUDE_VULKAN_01_HANDLE_RESULT(result, "failed to create fence");
}

Fence::~Fence()
{
  vkDestroyFence(CRUDE_VULKAN_01_HANDLE(m_device), m_handle, nullptr);
}

bool Fence::wait(uint64 timeout)
{
  constexpr bool waitAllFence = true;
  const VkResult result = vkWaitForFences(CRUDE_VULKAN_01_HANDLE(m_device), 1, &m_handle, waitAllFence, timeout);
  CRUDE_VULKAN_01_HANDLE_RESULT(result, "failed to wait for fence");
  return result != VK_TIMEOUT;
}

bool Fence::reset()
{
  const VkResult result = vkResetFences(CRUDE_VULKAN_01_HANDLE(m_device), 1u, &m_handle);
  CRUDE_VULKAN_01_HANDLE_RESULT(result, "failed to reset fence");
  return result != VK_ERROR_OUT_OF_DEVICE_MEMORY;
}

}
