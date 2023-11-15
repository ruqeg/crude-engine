#include "buffer.hpp"
#include "device.hpp"

namespace crude_vulkan_01
{

Buffer_Create_Info::Buffer_Create_Info(std::shared_ptr<const Device>  device,
                                       VkDeviceSize                   size,
                                       VkBufferUsageFlags             usage,
                                       VkSharingMode                  sharingMode,
                                       const std::vector<uint32>&     queueFamilyIndices)
  :
  device(device),
  size(size),
  usage(usage),
  sharingMode(sharingMode),
  queueFamilyIndices(queueFamilyIndices)
{}

Buffer::Buffer(const Buffer_Create_Info& createInfo)
{
  m_device = createInfo.device;

  VkBufferCreateInfo vkCreateInfo{};
  vkCreateInfo.sType                  = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  vkCreateInfo.pNext                  = nullptr;
  vkCreateInfo.flags                  = 0u;
  vkCreateInfo.size                   = createInfo.size;
  vkCreateInfo.usage                  = createInfo.usage;
  vkCreateInfo.sharingMode            = createInfo.sharingMode;
  vkCreateInfo.queueFamilyIndexCount  = static_cast<uint32>(createInfo.queueFamilyIndices.size());
  vkCreateInfo.pQueueFamilyIndices    = createInfo.queueFamilyIndices.data();

  VkResult result = vkCreateBuffer(CRUDE_VULKAN_01_HANDLE(m_device), &vkCreateInfo, nullptr, &m_handle);
  CRUDE_VULKAN_01_HANDLE_RESULT(result, "failed to create buffer");
}

Buffer::~Buffer()
{
  vkDestroyBuffer(CRUDE_VULKAN_01_HANDLE(m_device), m_handle, nullptr);
}

VkMemoryRequirements Buffer::getMemoryRequirements() const
{
  VkMemoryRequirements memRequirements;
  vkGetBufferMemoryRequirements(CRUDE_VULKAN_01_HANDLE(m_device), m_handle, &memRequirements);
  return memRequirements;
}
}
