#include "device_memory.hpp"
#include "device.hpp"
#include "buffer.hpp"
#include "image.hpp"
#include "physical_device.hpp"

namespace crude_engine
{


Device_Memory_Allocate_Info::Device_Memory_Allocate_Info(std::shared_ptr<const Device>  device,
                                                         VkDeviceSize                   allocationSize,
                                                         uint32                         memoryTypeIndex)
  :
  device(device),
  allocationSize(allocationSize),
  memoryTypeIndex(memoryTypeIndex)
{}

Device_Memory_Allocate_Info::Device_Memory_Allocate_Info(std::shared_ptr<const Device>  device,
                                                         VkDeviceSize                   allocationSize,
                                                         uint32                         memoryTypeFilter,
                                                         VkMemoryPropertyFlags          memoryProperties)
  :
  device(device),
  allocationSize(allocationSize)
{
  VkPhysicalDeviceMemoryProperties memProperties = device->getPhysicalDevice()->getMemoryProperties();

  this->memoryTypeIndex = -1;
  for (uint32 i = 0u; i < memProperties.memoryTypeCount; i++)
  {
    if (memoryTypeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & memoryProperties) == memoryProperties)
    {
      this->memoryTypeIndex = i;
    }
  }

  if (this->memoryTypeIndex == -1)  CRUDE_VULKAN_01_ERROR("failed to find depth memory type");
}

Device_Memory::Device_Memory(const Device_Memory_Allocate_Info& createInfo)
{
  m_device = createInfo.device;

  VkMemoryAllocateInfo vkAllocateInfo{};
  vkAllocateInfo.sType            = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  vkAllocateInfo.pNext            = nullptr;

  vkAllocateInfo.allocationSize   = createInfo.allocationSize;
  vkAllocateInfo.memoryTypeIndex  = createInfo.memoryTypeIndex;

  VkResult result = vkAllocateMemory(CRUDE_VULKAN_01_HANDLE(m_device), &vkAllocateInfo, nullptr, &m_handle);
  CRUDE_VULKAN_01_HANDLE_RESULT(result, "failed to allocate memory");
}

void Device_Memory::bind(Image& image, VkDeviceSize offset)
{
  vkBindImageMemory(CRUDE_VULKAN_01_HANDLE(m_device), image.getHandle(), m_handle, offset);
}

void Device_Memory::bind(Buffer& buffer, VkDeviceSize offset)
{
  vkBindBufferMemory(CRUDE_VULKAN_01_HANDLE(m_device), buffer.getHandle(), m_handle, offset);
}

std::optional<void*> Device_Memory::map(VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags)
{
  void* dst;
  VkResult result = vkMapMemory(CRUDE_VULKAN_01_HANDLE(m_device), m_handle, offset, size, flags, &dst);
  if (result != VK_SUCCESS) return std::nullopt;
  return dst;
}

void Device_Memory::unmap()
{
  vkUnmapMemory(CRUDE_VULKAN_01_HANDLE(m_device), m_handle);
}

Device_Memory::~Device_Memory()
{
  vkFreeMemory(CRUDE_VULKAN_01_HANDLE(m_device), m_handle, nullptr);
}

}