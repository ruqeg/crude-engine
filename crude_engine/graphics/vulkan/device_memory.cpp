#include <vulkan/vulkan.hpp>

module crude_engine.graphics.vulkan.device_memory;

import crude_engine.graphics.vulkan.device;
import crude_engine.graphics.vulkan.buffer;
import crude_engine.graphics.vulkan.image;
import crude_engine.graphics.vulkan.physical_device;
import crude_engine.graphics.vulkan.vulkan_utils;

namespace crude_engine
{

Device_Memory::Device_Memory(Shared_Ptr<const Device>  device,
                            VkDeviceSize              allocationSize,
                            uint32                    memoryTypeFilter,
                            VkMemoryPropertyFlags     memoryProperties)
  :
  m_device(device)
{
  int64 memoryTypeIndex;

  VkPhysicalDeviceMemoryProperties memProperties = m_device->getPhysicalDevice()->getMemoryProperties();

  memoryTypeIndex = -1;
  for (uint32 i = 0u; i < memProperties.memoryTypeCount; i++)
  {
    if (memoryTypeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & memoryProperties) == memoryProperties)
    {
      memoryTypeIndex = i;
    }
  }

  if (memoryTypeIndex == -1)
  {
    vulkanHandleError("failed to find depth memory type");
  }

  initalize(allocationSize, memoryTypeIndex);
}

Device_Memory::Device_Memory(Shared_Ptr<const Device>  device,
                            VkDeviceSize               allocationSize,
                            uint32                     memoryTypeIndex)
  :
  m_device(device)
{
  initalize(allocationSize, memoryTypeIndex);
}

void Device_Memory::initalize(VkDeviceSize allocationSize, uint32 memoryTypeIndex)
{
  VkMemoryAllocateInfo vkAllocateInfo{};
  vkAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  vkAllocateInfo.pNext = nullptr;

  vkAllocateInfo.allocationSize = allocationSize;
  vkAllocateInfo.memoryTypeIndex = memoryTypeIndex;

  VkResult result = vkAllocateMemory(m_device->getHandle(), &vkAllocateInfo, getPVkAllocationCallbacks(), &m_handle);
  vulkanHandleResult(result, "failed to allocate memory");
}

void Device_Memory::bind(Image& image, VkDeviceSize offset)
{
  vkBindImageMemory(m_device->getHandle(), image.getHandle(), m_handle, offset);
}

void Device_Memory::bind(Buffer& buffer, VkDeviceSize offset)
{
  vkBindBufferMemory(m_device->getHandle(), buffer.getHandle(), m_handle, offset);
}

Optional<void*> Device_Memory::map(VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags)
{
  void* dst;
  VkResult result = vkMapMemory(m_device->getHandle(), m_handle, offset, size, flags, &dst);
  if (result != VK_SUCCESS) return nullopt;
  return dst;
}

void Device_Memory::unmap()
{
  vkUnmapMemory(m_device->getHandle(), m_handle);
}

Device_Memory::~Device_Memory()
{
  vkFreeMemory(m_device->getHandle(), m_handle, getPVkAllocationCallbacks());
}

}