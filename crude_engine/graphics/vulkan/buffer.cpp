#include <vulkan/vulkan.hpp>

module crude_engine.graphics.vulkan.buffer;

import crude_engine.graphics.vulkan.device;
import crude_engine.graphics.vulkan.vulkan_utils;

namespace crude_engine
{

Buffer::Buffer(Shared_Ptr<const Device>  device,
  VkDeviceSize              size,
  VkBufferUsageFlags        usage,
  VkSharingMode             sharingMode,
  span<uint32>              queueFamilyIndices)
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
