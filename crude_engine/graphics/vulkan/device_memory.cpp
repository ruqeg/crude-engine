#include <vulkan/vulkan.hpp>

module crude.gfx.vk.device_memory;

import crude.gfx.vk.device;
import crude.gfx.vk.buffer;
import crude.gfx.vk.image;
import crude.gfx.vk.physical_device;
import crude.gfx.vk.vulkan_utils;

namespace crude::gfx::vk
{

Device_Memory::Device_Memory(core::shared_ptr<const Device>  device,
                             VkDeviceSize                    allocationSize,
                             core::uint32                    memoryTypeFilter,
                             VkMemoryPropertyFlags           memoryProperties,
                             const vk::Structure_Chain&      extendedMemoryInfo)
  : m_device{ device }
{
  core::int64 memoryTypeIndex;

  VkPhysicalDeviceMemoryProperties memProperties = m_device->getPhysicalDevice()->getMemoryProperties();

  // !TODO
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
    vulkanHandleError("failed to find device memory type");
  }

  initalize(allocationSize, memoryTypeIndex, extendedMemoryInfo);
}

Device_Memory::Device_Memory(core::shared_ptr<const Device>  device,
                             VkDeviceSize                    allocationSize,
                             core::uint32                    memoryTypeIndex,
                             const vk::Structure_Chain&      extendedMemoryInfo)
  : m_device{ device }
{
  initalize(allocationSize, memoryTypeIndex, extendedMemoryInfo);
}

Device_Memory::Device_Memory(core::shared_ptr<const Device>  device, 
                             VkMemoryRequirements            memoryRequirements, 
                             VkMemoryPropertyFlags           memoryProperties,
                             const vk::Structure_Chain&      extendedMemoryInfo)
  : Device_Memory{ device, memoryRequirements.size, memoryRequirements.memoryTypeBits, memoryProperties, extendedMemoryInfo }
{}

void Device_Memory::initalize(VkDeviceSize allocationSize, core::uint32 memoryTypeIndex, const vk::Structure_Chain& extendedMemoryInfo)
{
  VkMemoryAllocateInfo vkAllocateInfo{};
  vkAllocateInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  vkAllocateInfo.pNext           = extendedMemoryInfo.getHeadNode();
  vkAllocateInfo.allocationSize  = allocationSize;
  vkAllocateInfo.memoryTypeIndex = memoryTypeIndex;

  VkResult result = vkAllocateMemory(m_device->getHandle(), &vkAllocateInfo, getPVkAllocationCallbacks(), &m_handle);
  vulkanHandleResult(result, "failed to allocate memory");
}

void Device_Memory::bind(core::shared_ptr<Buffer> buffer, VkDeviceSize offset)
{
  vkBindBufferMemory(m_device->getHandle(), buffer->getHandle(), m_handle, offset);
}

void Device_Memory::bind(core::shared_ptr<Image> image, VkDeviceSize offset)
{
  vkBindImageMemory(m_device->getHandle(), image->getHandle(), m_handle, offset);
}

core::optional<void*> Device_Memory::map(VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags)
{
  void* dst;
  VkResult result = vkMapMemory(m_device->getHandle(), m_handle, offset, size, flags, &dst);
  if (result != VK_SUCCESS) return core::nullopt;
  return dst;
}

void* Device_Memory::mapUnsafe(VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags)
{
  void* dst;
  VkResult result = vkMapMemory(m_device->getHandle(), m_handle, offset, size, flags, &dst);
  if (result != VK_SUCCESS) nullptr;
  return dst;
}

void Device_Memory::unmap()
{
  vkUnmapMemory(m_device->getHandle(), m_handle);
}

core::shared_ptr<const Device> Device_Memory::getDevice() const
{
  return m_device;
}

Device_Memory::~Device_Memory()
{
  vkFreeMemory(m_device->getHandle(), m_handle, getPVkAllocationCallbacks());
}

}