#include <vulkan/vulkan.hpp>

module crude_engine.graphics.vulkan.queue;

import crude_engine.graphics.vulkan.device;
import crude_engine.graphics.vulkan.command_buffer;
import crude_engine.graphics.vulkan.fence;
import crude_engine.graphics.vulkan.semaphore;
import crude_engine.graphics.vulkan.swap_chain;
import crude_engine.core.algorithms;
import crude_engine.core.std_containers_heap;

namespace crude_engine
{

Queue::Queue(uint32 familyIndex, uint32 index)
  :
  m_familyIndex(familyIndex),
  m_index(index)
{}

bool Queue::sumbit(const span<Shared_Ptr<Command_Buffer>>&  commandBuffers,
                   const span<VkPipelineStageFlags>&        waitStageMasks,
                   const span<Shared_Ptr<Semaphore>>&       waitSemaphores,
                   const span<Shared_Ptr<Semaphore>>&       signalSemaphores,
                   const Optional<Shared_Ptr<Fence>>&               fence)
{
  if (waitSemaphores.size() != waitStageMasks.size())
  {
    return false;
  }

  vector<VkCommandBuffer> commandBuffersHandles(commandBuffers.size());
  Algorithms::copyc(commandBuffers.begin(), commandBuffers.end(), commandBuffersHandles.begin(), [](auto& src, auto& dst) -> void {
    *dst = (*src)->getHandle();
  });

  vector<VkSemaphore> waitSemaphoreHandles(waitSemaphores.size());
  Algorithms::copyc(waitSemaphores.begin(), waitSemaphores.end(), waitSemaphoreHandles.begin(), [](auto& src, auto& dst) -> void {
    *dst = (*src)->getHandle();
    });

  vector<VkSemaphore> signalSemaphoreHandles(signalSemaphores.size());
  Algorithms::copyc(signalSemaphores.begin(), signalSemaphores.end(), signalSemaphoreHandles.begin(), [](auto& src, auto& dst) -> void {
    *dst = (*src)->getHandle();
    });

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

  VkResult result = vkQueueSubmit(m_handle, 1u, &vkSumbitInfo, fence.hasValue() ? fence.value()->getHandle() : VK_NULL_HANDLE);
  return result == VK_SUCCESS;
}

Queue_Present_Result Queue::present(const span<Shared_Ptr<Swap_Chain>>&  swapchains,
                                    const span<uint32>&                  imageIndices,
                                    const span<Shared_Ptr<Semaphore>>&   waitSemaphores)
{
  vector<VkSemaphore> waitSemaphoreHandles(waitSemaphores.size());
  Algorithms::copyc(waitSemaphores.begin(), waitSemaphores.end(), waitSemaphoreHandles.begin(), [](auto& src, auto& dst) -> void {
    *dst = (*src)->getHandle();
    });

  vector<VkSwapchainKHR> swapchainHandles(swapchains.size());
  Algorithms::copyc(swapchains.begin(), swapchains.end(), swapchainHandles.begin(), [](auto& src, auto& dst) -> void {
    *dst = (*src)->getHandle();
    });

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
