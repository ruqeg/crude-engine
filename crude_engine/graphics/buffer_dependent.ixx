module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.buffer_dependent;

export import crude.graphics.buffer;
export import crude.graphics.device_memory;
export import crude.graphics.command_pool;
export import crude.graphics.queue;
export import crude.core.std_containers_stack;
export import crude.core.std_containers_heap;
export import crude.graphics.command_buffer;
export import crude.graphics.device;
export import crude.graphics.fence;

export namespace crude::graphics
{

template<class T>
class Buffer_Dependent
{
public:
  explicit Buffer_Dependent(core::shared_ptr<Queue>         queue, 
                            core::shared_ptr<Command_Pool>  commandPool,
                            core::span<const T>             data,
                            VkBufferUsageFlagBits           additionalUsageFlag);
  core::shared_ptr<Buffer> getBuffer() { return m_buffer; }
  core::shared_ptr<Device_Memory> getDeviceMemory() { return m_bufferMemory; }
private:
  core::shared_ptr<Buffer>         m_buffer;
  core::shared_ptr<Device_Memory>  m_bufferMemory;
};


template<class T>
Buffer_Dependent<T>::Buffer_Dependent(core::shared_ptr<Queue>         queue, 
                                      core::shared_ptr<Command_Pool>  commandPool, 
                                      core::span<const T>             data,
                                      VkBufferUsageFlagBits           additionalUsageFlag)
{
  const VkDeviceSize bufferSize = data.size_bytes();
  core::shared_ptr<const Device> device = commandPool->getDevice();

  auto stagingBuffer = core::allocateShared<Buffer>(device, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
  auto staggingBufferMemory = core::allocateShared<Device_Memory>(device, stagingBuffer->getMemoryRequirements(), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

  staggingBufferMemory->bind(*stagingBuffer);
  core::Optional<void*> mappedData = staggingBufferMemory->map();
  if (mappedData.hasValue())
  {
    std::memcpy(mappedData.value(), data.data(), bufferSize);
    staggingBufferMemory->unmap();
  }

  m_buffer = core::allocateShared<Buffer>(device, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | additionalUsageFlag);
  m_bufferMemory = core::allocateShared<Device_Memory>(device, m_buffer->getMemoryRequirements(), VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  m_bufferMemory->bind(*m_buffer);

  auto commandBuffer = core::allocateShared<Command_Buffer>(device, commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
  commandBuffer->begin();
  commandBuffer->copyBuffer(stagingBuffer, m_buffer, bufferSize);
  commandBuffer->end();
  queue->sumbit(core::span(&commandBuffer, 1u));
  queue->waitIdle();
}

}
