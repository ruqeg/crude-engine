#include "command_buffer.hpp"
#include "command_pool.hpp"
#include "device.hpp"
#include "image.hpp"

namespace crude_vulkan_01
{

Command_Buffer::Command_Buffer(const Command_Buffer_Create_Info& createInfo)
{
  m_device       = createInfo.device;
  m_commandPool  = createInfo.commandPool;

  VkCommandBufferAllocateInfo vkAllocateInfo{};
  vkAllocateInfo.sType               = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  vkAllocateInfo.pNext               = nullptr;

  vkAllocateInfo.commandPool         = CRUDE_VULKAN_01_HANDLE(m_commandPool);
  vkAllocateInfo.level               = createInfo.level;
  vkAllocateInfo.commandBufferCount  = 1u;

  VkResult result = vkAllocateCommandBuffers(CRUDE_VULKAN_01_HANDLE(m_device), &vkAllocateInfo, &m_handle);
  CRUDE_VULKAN_01_HANDLE_RESULT(result, "failed to allocate command buffer");
}

bool Command_Buffer::begin(VkCommandBufferUsageFlags flags)
{
  VkCommandBufferBeginInfo vkBeginInfo{};
  vkBeginInfo.sType             = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  vkBeginInfo.pNext             = nullptr;
  vkBeginInfo.pInheritanceInfo  = nullptr;
  vkBeginInfo.flags             = flags;
  VkResult result = vkBeginCommandBuffer(m_handle, &vkBeginInfo);
  return result == VK_SUCCESS;
}

bool Command_Buffer::end()
{
  VkResult result = vkEndCommandBuffer(m_handle);
  return result == VK_SUCCESS;
}

void Command_Buffer::barrier(VkPipelineStageFlags srcStage, VkPipelineStageFlags dstStage, const std::vector<Image_Memory_Barrier>& imageMemoryBarriers)
{
  std::vector<VkImageMemoryBarrier> vkImageMemoryBarriers(imageMemoryBarriers.begin(), imageMemoryBarriers.end());
  vkCmdPipelineBarrier(m_handle, srcStage, dstStage, 0u, 0u, nullptr, 0u, nullptr, vkImageMemoryBarriers.size(), vkImageMemoryBarriers.data());
}

Command_Buffer::~Command_Buffer()
{
  vkFreeCommandBuffers(CRUDE_VULKAN_01_HANDLE(m_device), CRUDE_VULKAN_01_HANDLE(m_commandPool), 1u, &m_handle);
}

}