#include <vulkan/vulkan.hpp>

module crude.graphics.vulkan.buffer;

import crude.graphics.vulkan.device;
import crude.graphics.vulkan.vulkan_utils;

namespace crude::graphics
{

Buffer::Buffer(core::Shared_Ptr<const Device>  device,
               VkDeviceSize                    size,
               VkBufferUsageFlags              usage,
               VkSharingMode                   sharingMode,
               core::span<core::uint32>        queueFamilyIndices)
  :
  m_device(device)
{
  VkBufferCreateInfo vkCreateInfo{};
  vkCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  vkCreateInfo.pNext = nullptr;
  vkCreateInfo.flags = 0u;
  vkCreateInfo.size = size;
  vkCreateInfo.usage = usage;
  vkCreateInfo.sharingMode = sharingMode;
  vkCreateInfo.queueFamilyIndexCount = queueFamilyIndices.size();
  vkCreateInfo.pQueueFamilyIndices = queueFamilyIndices.data();

  VkResult result = vkCreateBuffer(m_device->getHandle(), &vkCreateInfo, getPVkAllocationCallbacks(), &m_handle);
  vulkanHandleResult(result, "failed to create buffer");
}

Buffer::~Buffer()
{
  vkDestroyBuffer(m_device->getHandle(), m_handle, getPVkAllocationCallbacks());
}

VkMemoryRequirements Buffer::getMemoryRequirements() const
{
  VkMemoryRequirements memRequirements;
  vkGetBufferMemoryRequirements(m_device->getHandle(), m_handle, &memRequirements);
  return memRequirements;
}
}
