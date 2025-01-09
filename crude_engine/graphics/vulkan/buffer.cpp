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

namespace crude::gfx::vk
{

Buffer::Buffer(const Initialize& info)
  : m_device(info.device)
  , m_size(info.size)
{
  VkBufferCreateInfo vkCreateInfo{};
  vkCreateInfo.sType                  = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  vkCreateInfo.pNext                  = nullptr;
  vkCreateInfo.flags                  = 0u;
  vkCreateInfo.size                   = info.size;
  vkCreateInfo.usage                  = info.usage;
  vkCreateInfo.sharingMode            = info.queueFamilyIndices.size() > 1 ? VK_SHARING_MODE_CONCURRENT : VK_SHARING_MODE_EXCLUSIVE;
  vkCreateInfo.queueFamilyIndexCount  = info.queueFamilyIndices.size();
  vkCreateInfo.pQueueFamilyIndices    = info.queueFamilyIndices.data();

  VkResult result = vkCreateBuffer(m_device->getHandle(), &vkCreateInfo, getPVkAllocationCallbacks(), &m_handle);
  vulkanHandleResult(result, "failed to create buffer");

  m_memory = core::allocateShared<Device_Memory>(m_device, getMemoryRequirements(), info.memoryFlags);
  m_memory->bind(core::makeUnsafeSharedWithEmptyDestructor(this));
}

void Buffer::stagedUpload(core::shared_ptr<Command_Buffer> commandBuffer, const void* data, VkDeviceSize size) noexcept
{
  core::shared_ptr<Staging_Buffer> stagingBuffer = core::allocateShared<Staging_Buffer>(m_device, data, size);
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

  auto vkGetBufferDeviceAddressEXT = getDeviceExtension<PFN_vkGetBufferDeviceAddressEXT>(m_device);
  if (vkGetBufferDeviceAddressEXT)
  {
    return vkGetBufferDeviceAddressEXT(m_device->getHandle(), &info);
  }
  return cDeviceAddressNull;
}

VkDeviceSize Buffer::getSize() const
{
  return m_size;
}

}
