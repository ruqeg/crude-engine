#include <vulkan/vulkan.hpp>
#include <memory>

module crude.gfx.vk.buffer;

import crude.gfx.vk.vulkan_utils;
import crude.gfx.vk.device;
import crude.gfx.vk.staging_buffer;
import crude.gfx.vk.device_memory;
import crude.gfx.vk.queue;
import crude.gfx.vk.command_pool;
import crude.gfx.vk.flush;
import crude.gfx.vk.constants;
import crude.core.logger;
import crude.gfx.vk.extension;
import crude.gfx.vk.command_buffer;

namespace crude::gfx::vk
{

Buffer::Buffer(const Initialize& info)
  : m_device(info.device)
  , m_size(info.size)
{
  VkBufferCreateInfo bufferCreateInfo{};
  bufferCreateInfo.sType                  = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferCreateInfo.pNext                  = nullptr;
  bufferCreateInfo.flags                  = 0u;
  bufferCreateInfo.size                   = info.size;
  bufferCreateInfo.usage                  = info.usage;
  bufferCreateInfo.sharingMode            = info.queueFamilyIndices.size() > 1 ? VK_SHARING_MODE_CONCURRENT : VK_SHARING_MODE_EXCLUSIVE;
  bufferCreateInfo.queueFamilyIndexCount  = info.queueFamilyIndices.size();
  bufferCreateInfo.pQueueFamilyIndices    = info.queueFamilyIndices.data();

  VkResult result = vkCreateBuffer(m_device->getHandle(), &bufferCreateInfo, getPVkAllocationCallbacks(), &m_handle);
  vulkanHandleResult(result, "failed to create buffer");

  VkMemoryAllocateFlagsInfo memoryAllocateFlagsInfo{ .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO };
  if (bufferCreateInfo.usage & VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT_KHR)
  {
    memoryAllocateFlagsInfo.flags |= VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT_KHR;
  }

  vk::Structure_Chain extendedMemoryInfo;
  extendedMemoryInfo.linkNode(memoryAllocateFlagsInfo);

  m_memory = core::allocateShared<Device_Memory>(m_device, getMemoryRequirements(), info.memoryFlags, extendedMemoryInfo);
  m_memory->bind(core::makeUnsafeSharedWithEmptyDestructor(this));
}

void Buffer::stagedUpload(core::shared_ptr<Command_Buffer> commandBuffer, const void* data, VkDeviceSize size) noexcept
{
  core::shared_ptr<Staging_Buffer> stagingBuffer = core::allocateShared<Staging_Buffer>(m_device, size, data);
  commandBuffer->begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
  copyTransfer(commandBuffer, stagingBuffer);
  commandBuffer->end();
  flush(commandBuffer);
}

void Buffer::copyHost(const void* data, VkDeviceSize size) noexcept
{
  core::optional<void*> mappedData = m_memory->map();
  if (mappedData.has_value())
  {
    std::memcpy(mappedData.value(), data, size);
    m_memory->unmap();
  }
}

core::shared_ptr<Device_Memory> Buffer::getMemory()
{
  return m_memory;
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
  const VkDeviceSize wholeSize = std::min(m_size, srcBuffer->getSize());
  if (VK_WHOLE_SIZE != size)
  {
    size = std::min(size, wholeSize);
  }

  VkBufferCopy region;
  region.srcOffset = srcOffset;
  region.dstOffset = dstOffset;
  region.size      = (VK_WHOLE_SIZE == size) ? wholeSize : size;
  commandBuffer->copyBuffer(srcBuffer, core::makeUnsafeSharedWithEmptyDestructor(this), region);
}

void Buffer::bindMemory(core::shared_ptr<Device_Memory> memory, VkDeviceSize offset)
{
  memory->bind(shared_from_this());
  m_memory = memory;
}

VkMemoryRequirements Buffer::getMemoryRequirements() const
{
  VkMemoryRequirements memRequirements;
  vkGetBufferMemoryRequirements(m_device->getHandle(), m_handle, &memRequirements);
  return memRequirements;
}

VkDeviceAddress Buffer::getDeviceAddress() const
{
  VkBufferDeviceAddressInfoEXT info{};
  info.sType  = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO_EXT;
  info.pNext  = nullptr;
  info.buffer = m_handle;

  auto vkGetBufferDeviceAddressKHR = getDeviceExtension<PFN_vkGetBufferDeviceAddressKHR>(m_device);
  if (vkGetBufferDeviceAddressKHR)
  {
    return vkGetBufferDeviceAddressKHR(m_device->getHandle(), &info);
  } 
  return cDeviceAddressNull;
}

VkDeviceSize Buffer::getSize() const
{
  return m_size;
}

}