#include <vulkan/vulkan.hpp>

module crude.graphics.buffer;

import crude.graphics.device;
import crude.graphics.staging_buffer;
import crude.graphics.vulkan_utils;

namespace crude::graphics
{

Buffer::Buffer(core::shared_ptr<const Device>  device,
               VkDeviceSize                    size,
               VkBufferUsageFlags              usage,
               VkMemoryPropertyFlags           memoryFlags)
  :
  Buffer(device, size, usage, memoryFlags, VK_SHARING_MODE_EXCLUSIVE, {})
{}

Buffer::Buffer(core::shared_ptr<const Device>  device,
               VkDeviceSize                    size,
               VkBufferUsageFlags              usage,
               VkMemoryPropertyFlags           memoryFlags,
               core::span<core::uint32>        queueFamilyIndices)
  :
  Buffer(device, size, usage, memoryFlags, VK_SHARING_MODE_CONCURRENT, queueFamilyIndices)
{}

Buffer::Buffer(core::shared_ptr<const Device>  device,
               VkDeviceSize                    size,
               VkBufferUsageFlags              usage,
               VkMemoryPropertyFlags           memoryFlags,
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

  core::shared_ptr<Device_Memory> memory = core::allocateShared<Device_Memory>(m_device, getMemoryRequirements(), memoryFlags);
  bindMemory(memory);
}

Buffer::~Buffer()
{
  vkDestroyBuffer(m_device->getHandle(), m_handle, getPVkAllocationCallbacks());
}

void Buffer::copyTransfer(core::shared_ptr<Command_Buffer>        commandBuffer,
                          core::shared_ptr<const Staging_Buffer>  srcBuffer,
                          VkDeviceSize                            srcOffset,
                          VkDeviceSize                            dstOffset,
                          VkDeviceSize                            size)
{
  const VkDeviceSize wholeSize = std::min(getMemoryRequirements().size, srcBuffer->getMemoryRequirements().size);
  if (VK_WHOLE_SIZE != size)
  {
    size = std::min(size, wholeSize);
  }

  VkBufferCopy region;
  region.srcOffset = srcOffset;
  region.dstOffset = dstOffset;
  region.size      = (VK_WHOLE_SIZE == size) ? wholeSize : size;
  core::shared_ptr<Buffer> self = core::shared_ptr<Buffer>(this, [](Buffer*) {});
  commandBuffer->copyBuffer(srcBuffer, self, region);
}

void Buffer::bindMemory(core::shared_ptr<Device_Memory> memory, VkDeviceSize offset)
{
  memory->bind(*this);
  m_memory = memory;
}

VkMemoryRequirements Buffer::getMemoryRequirements() const
{
  VkMemoryRequirements memRequirements;
  vkGetBufferMemoryRequirements(m_device->getHandle(), m_handle, &memRequirements);
  return memRequirements;
}
}
