#include <graphics/vulkan/buffer.hpp>
#include <graphics/vulkan/device.hpp>

namespace crude_engine
{

Buffer::Buffer(Shared_Ptr<const Device>  device,
               VkDeviceSize              size,
               VkBufferUsageFlags        usage,
               VkSharingMode             sharingMode,
               Array_Unsafe<uint32>      queueFamilyIndices)
  :
  m_device(device)
{
  VkBufferCreateInfo vkCreateInfo{};
  vkCreateInfo.sType                  = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  vkCreateInfo.pNext                  = nullptr;
  vkCreateInfo.flags                  = 0u;
  vkCreateInfo.size                   = size;
  vkCreateInfo.usage                  = usage;
  vkCreateInfo.sharingMode            = sharingMode;
  vkCreateInfo.queueFamilyIndexCount  = queueFamilyIndices.size();
  vkCreateInfo.pQueueFamilyIndices    = queueFamilyIndices.data();

  VkResult result = vkCreateBuffer(CRUDE_OBJECT_HANDLE(m_device), &vkCreateInfo, &getVkAllocationCallbacks(), &m_handle);
  CRUDE_VULKAN_HANDLE_RESULT(result, "failed to create buffer");
}

Buffer::~Buffer()
{
  vkDestroyBuffer(CRUDE_OBJECT_HANDLE(m_device), m_handle, &getVkAllocationCallbacks());
}

VkMemoryRequirements Buffer::getMemoryRequirements() const
{
  VkMemoryRequirements memRequirements;
  vkGetBufferMemoryRequirements(CRUDE_OBJECT_HANDLE(m_device), m_handle, &memRequirements);
  return memRequirements;
}
}
