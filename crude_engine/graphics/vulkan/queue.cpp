#include "queue.hpp"
#include "command_buffer.hpp"
#include "fence.hpp"
#include "semaphore.hpp"
#include "swap_chain.hpp"

namespace crude_engine
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

bool Queue::sumbit(const std::vector<std::shared_ptr<Command_Buffer>>&  commandBuffers,
                   const std::vector<VkPipelineStageFlags>&             waitStageMasks,
                   const std::vector<std::shared_ptr<Semaphore>>&       waitSemaphores,
                   const std::vector<std::shared_ptr<Semaphore>>&       signalSemaphores,
                   const std::optional<std::shared_ptr<Fence>>&         fence)
{

  if (waitSemaphores.size() != waitStageMasks.size())
  {
    return false;
  }

  std::vector<VkCommandBuffer> commandBuffersHandles(commandBuffers.size());
  for (uint32 i = 0; i < commandBuffers.size(); ++i)
  {
    commandBuffersHandles[i] = CRUDE_VULKAN_01_HANDLE(commandBuffers[i]);
  }

  std::vector<VkSemaphore> waitSemaphoreHandles(waitSemaphores.size());
  for (uint32 i = 0u; i < waitSemaphoreHandles.size(); ++i)
    waitSemaphoreHandles[i] = CRUDE_VULKAN_01_HANDLE(waitSemaphores[i]);

  std::vector<VkSemaphore> signalSemaphoreHandles(signalSemaphores.size());
  for (uint32 i = 0u; i < signalSemaphoreHandles.size(); ++i)
    signalSemaphoreHandles[i] = CRUDE_VULKAN_01_HANDLE(signalSemaphores[i]);

  VkSubmitInfo vkSumbitInfo;
  vkSumbitInfo.sType                 = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  vkSumbitInfo.pNext                 = nullptr;
  vkSumbitInfo.waitSemaphoreCount    = static_cast<uint32>(waitSemaphoreHandles.size());
  vkSumbitInfo.pWaitSemaphores       = waitSemaphoreHandles.data();
  vkSumbitInfo.pWaitDstStageMask     = waitStageMasks.data();
  vkSumbitInfo.commandBufferCount    = static_cast<uint32>(commandBuffersHandles.size());
  vkSumbitInfo.pCommandBuffers       = commandBuffersHandles.data();
  vkSumbitInfo.signalSemaphoreCount  = static_cast<uint32>(signalSemaphoreHandles.size());
  vkSumbitInfo.pSignalSemaphores     = signalSemaphoreHandles.data();

  VkResult result = vkQueueSubmit(m_handle, 1u, &vkSumbitInfo, CRUDE_VULKAN_01_OPTIONAL_HANDLE_OR_NULL(fence));
  return result == VK_SUCCESS;
}

Queue_Present_Result Queue::present(const std::vector<std::shared_ptr<Swap_Chain>>&  swapchains,
                                    const std::vector<uint32>&                       imageIndices, 
                                    const std::vector<std::shared_ptr<Semaphore>>&   waitSemaphores)
{
  std::vector<VkSemaphore> waitSemaphoreHandles(waitSemaphores.size());
  for (uint32 i = 0u; i < waitSemaphoreHandles.size(); ++i)
    waitSemaphoreHandles[i] = CRUDE_VULKAN_01_HANDLE(waitSemaphores[i]);

  std::vector<VkSwapchainKHR> swapchainHandles(swapchains.size());
  for (uint32 i = 0u; i < swapchainHandles.size(); ++i)
    swapchainHandles[i] = CRUDE_VULKAN_01_HANDLE(swapchains[i]);

  VkPresentInfoKHR vkPresentInfo{};
  vkPresentInfo.sType               = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  vkPresentInfo.pNext               = nullptr;
  vkPresentInfo.waitSemaphoreCount  = static_cast<uint32>(waitSemaphoreHandles.size());
  vkPresentInfo.pWaitSemaphores     = waitSemaphoreHandles.data();
  vkPresentInfo.swapchainCount      = static_cast<uint32>(swapchainHandles.size());
  vkPresentInfo.pSwapchains         = swapchainHandles.data();
  vkPresentInfo.pImageIndices       = imageIndices.data();
  vkPresentInfo.pResults            = nullptr; // !TODO

  const VkResult result = vkQueuePresentKHR(m_handle, &vkPresentInfo);
  return Queue_Present_Result(result);
}

void Queue::waitIdle()
{
  vkQueueWaitIdle(m_handle);
}

Queue_Present_Result::Queue_Present_Result(VkResult result)
  :
  m_result(result)
{}

bool Queue_Present_Result::outOfDate() const
{
  return m_result == VK_ERROR_OUT_OF_DATE_KHR;
}

bool Queue_Present_Result::suboptimal() const
{
  return m_result == VK_SUBOPTIMAL_KHR;
}

bool Queue_Present_Result::failed() const
{
  return m_result != VK_SUCCESS;
}

VkResult Queue_Present_Result::getResult() const
{
  return m_result;
}

}
