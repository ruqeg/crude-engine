#include "device_memory.hpp"
#include "device.hpp"
#include "image.hpp"

namespace crude_vulkan_01
{


Device_Memory_Allocate_Info::Device_Memory_Allocate_Info(std::shared_ptr<const Device>  device,
                                                         VkDeviceSize                   allocationSize,
                                                         uint32                         memoryTypeIndex)
  :
  device(device),
  allocationSize(allocationSize),
  memoryTypeIndex(memoryTypeIndex)
{}

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

Device_Memory::~Device_Memory()
{
  vkFreeMemory(CRUDE_VULKAN_01_HANDLE(m_device), m_handle, nullptr);
}

}