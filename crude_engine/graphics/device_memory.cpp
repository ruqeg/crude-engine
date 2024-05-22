#include <vulkan/vulkan.hpp>

module crude.graphics.vulkan.device_memory;

import crude.graphics.vulkan.device;
import crude.graphics.vulkan.buffer;
import crude.graphics.vulkan.image;
import crude.graphics.vulkan.physical_device;
import crude.graphics.vulkan.vulkan_utils;

namespace crude::graphics
{

Device_Memory::Device_Memory(core::Shared_Ptr<const Device>  device,
                             VkDeviceSize                    allocationSize,
                             core::uint32                    memoryTypeFilter,
                             VkMemoryPropertyFlags           memoryProperties)
  :
  m_device(device)
{
  core::int64 memoryTypeIndex;

  VkPhysicalDeviceMemoryProperties memProperties = m_device->getPhysicalDevice()->getMemoryProperties();

  memoryTypeIndex = -1;
  for (core::uint32 i = 0u; i < memProperties.memoryTypeCount; i++)
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

Device_Memory::Device_Memory(core::Shared_Ptr<const Device>  device,
                             VkDeviceSize                    allocationSize,
                             core::uint32                    memoryTypeIndex)
  :
  m_device(device)
{
  initalize(allocationSize, memoryTypeIndex);
}

void Device_Memory::initalize(VkDeviceSize allocationSize, core::uint32 memoryTypeIndex)
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

core::Optional<void*> Device_Memory::map(VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags)
{
  void* dst;
  VkResult result = vkMapMemory(m_device->getHandle(), m_handle, offset, size, flags, &dst);
  if (result != VK_SUCCESS) return core::nullopt;
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