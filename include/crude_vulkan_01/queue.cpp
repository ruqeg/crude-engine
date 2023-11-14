#include "queue.hpp"
#include "command_buffer.hpp"

namespace crude_vulkan_01
{
  
Queue_Create_Info::Queue_Create_Info(uint32 familyIndex, uint32 index)
  :
  familyIndex(familyIndex),
  index(index)
{}
  
Queue::Queue(const Queue_Create_Info& createInfo)
  :
  m_familyIndex(createInfo.familyIndex),
  m_index(createInfo.index)
{}

bool Queue::sumbit(const std::vector<std::shared_ptr<Command_Buffer>>& commandBuffers)
{
  std::vector<VkCommandBuffer> vkCommandBuffers(commandBuffers.size());
  for (uint32 i = 0; i < commandBuffers.size(); ++i)
  {
    vkCommandBuffers[i] = CRUDE_VULKAN_01_HANDLE(commandBuffers[i]);
  }

  VkSubmitInfo vkSumbitInfo;
  vkSumbitInfo.sType                 = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  vkSumbitInfo.pNext                 = nullptr;
  vkSumbitInfo.waitSemaphoreCount    = 0u; // !TODO
  vkSumbitInfo.pWaitSemaphores       = nullptr; // !TODO
  vkSumbitInfo.pWaitDstStageMask     = 0u; // !TODO
  vkSumbitInfo.commandBufferCount    = vkCommandBuffers.size();
  vkSumbitInfo.pCommandBuffers       = vkCommandBuffers.data();
  vkSumbitInfo.signalSemaphoreCount  = 0u; // !TODO
  vkSumbitInfo.pSignalSemaphores     = nullptr; // !TODO

  VkResult result = vkQueueSubmit(m_handle, 1u, &vkSumbitInfo, VK_NULL_HANDLE);
  return result == VK_SUCCESS;
}

void Queue::waitIdle()
{
  vkQueueWaitIdle(m_handle);
}

}
