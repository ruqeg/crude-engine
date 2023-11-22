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

}
