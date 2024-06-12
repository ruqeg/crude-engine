module;

#include <algorithm>
#include <vulkan/vulkan.h>

export module crude.graphics.buffer;

import crude.core.optional;
export import crude.graphics.device_memory;
export import crude.core.std_containers_stack;
export import crude.core.std_containers_heap;
export import crude.graphics.command_buffer;
import crude.graphics.vulkan_object;

export namespace crude::graphics
{

class Device;
class Command_Buffer;
class Device_Memory;
class Staging_Buffer;

class Buffer : public Vulkan_Object<VkBuffer>
{
public:
  explicit Buffer(core::shared_ptr<const Device>  device,
                  VkDeviceSize                    size,
                  VkBufferUsageFlags              usage,
                  VkMemoryPropertyFlags           memoryFlags);
  explicit Buffer(core::shared_ptr<const Device>  device,
                  VkDeviceSize                    size,
                  VkBufferUsageFlags              usage,
                  VkMemoryPropertyFlags           memoryFlags,
                  core::span<core::uint32>        queueFamilyIndices);
  ~Buffer();
public:
  void copyTransfer(core::shared_ptr<Command_Buffer>        commandBuffer, 
                    core::shared_ptr<const Staging_Buffer>  srcBuffer,
                    VkDeviceSize                            srcOffset = 0,
                    VkDeviceSize                            dstOffset = 0,
                    VkDeviceSize                            size = VK_WHOLE_SIZE);
  template<class T>
  void stagedUpload(core::shared_ptr<Command_Buffer> commandBuffer, core::span<const T> data);
  void bindMemory(core::shared_ptr<Device_Memory> memory, VkDeviceSize offset = 0);
  template<class T>
  void copyHost(core::span<const T> srcData) noexcept;
  VkMemoryRequirements getMemoryRequirements() const;
private:
  explicit Buffer(core::shared_ptr<const Device>  device,
                  VkDeviceSize                    size,
                  VkBufferUsageFlags              usage,
                  VkMemoryPropertyFlags           memoryFlags,
                  VkSharingMode                   sharingMode,
                  core::span<core::uint32>        queueFamilyIndices);
protected:
  core::shared_ptr<const Device>   m_device;
  core::shared_ptr<Device_Memory>  m_memory;
};

template<class T>
void Buffer::stagedUpload(core::shared_ptr<Command_Buffer> commandBuffer, core::span<const T> data)
{
  core::shared_ptr<Staging_Buffer> stagingBuffer = core::allocateShared<Staging_Buffer>(m_device, data);
  commandBuffer->begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
  copyTransfer(commandBuffer, stagingBuffer);
  commandBuffer->end();
}

template<class T>
void Buffer::copyHost(core::span<const T> srcData) noexcept
{
  core::Optional<void*> mappedData = m_memory->map();
  if (mappedData.hasValue())
  {
    std::memcpy(mappedData.value(), srcData.data(), srcData.size_bytes());
    m_memory->unmap();
  }
}

}
