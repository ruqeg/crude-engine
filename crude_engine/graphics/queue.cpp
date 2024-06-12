#include <vulkan/vulkan.hpp>

module crude.graphics.queue;

import crude.graphics.device;
import crude.graphics.command_buffer;
import crude.graphics.fence;
import crude.graphics.semaphore;
import crude.graphics.swap_chain;
import crude.core.algorithms;
import crude.core.std_containers_heap;

namespace crude::graphics
{

Queue::Queue(core::uint32 familyIndex, core::uint32 index)
  :
  m_familyIndex(familyIndex),
  m_index(index)
{}

bool Queue::sumbit(const core::span<core::shared_ptr<Command_Buffer>>&  commandBuffers,
                   const core::span<VkPipelineStageFlags>&              waitStageMasks,
                   const core::span<core::shared_ptr<Semaphore>>&       waitSemaphores,
                   const core::span<core::shared_ptr<Semaphore>>&       signalSemaphores,
                   const core::Optional<core::shared_ptr<Fence>>&       fence)
{
  if (waitSemaphores.size() != waitStageMasks.size())
  {
    return false;
  }

  core::vector<VkCommandBuffer> commandBuffersHandles(commandBuffers.size());
  core::Algorithms::copyc(commandBuffers.begin(), commandBuffers.end(), commandBuffersHandles.begin(), [](auto& src, auto& dst) -> void {
    *dst = (*src)->getHandle();
  });

  core::vector<VkSemaphore> waitSemaphoreHandles(waitSemaphores.size());
  core::Algorithms::copyc(waitSemaphores.begin(), waitSemaphores.end(), waitSemaphoreHandles.begin(), [](auto& src, auto& dst) -> void {
    *dst = (*src)->getHandle();
    });

  core::vector<VkSemaphore> signalSemaphoreHandles(signalSemaphores.size());
  core::Algorithms::copyc(signalSemaphores.begin(), signalSemaphores.end(), signalSemaphoreHandles.begin(), [](auto& src, auto& dst) -> void {
    *dst = (*src)->getHandle();
    });

  VkSubmitInfo vkSumbitInfo;
  vkSumbitInfo.sType                 = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  vkSumbitInfo.pNext                 = nullptr;
  vkSumbitInfo.waitSemaphoreCount    = static_cast<core::uint32>(waitSemaphoreHandles.size());
  vkSumbitInfo.pWaitSemaphores       = waitSemaphoreHandles.data();
  vkSumbitInfo.pWaitDstStageMask     = waitStageMasks.data();
  vkSumbitInfo.commandBufferCount    = static_cast<core::uint32>(commandBuffersHandles.size());
  vkSumbitInfo.pCommandBuffers       = commandBuffersHandles.data();
  vkSumbitInfo.signalSemaphoreCount  = static_cast<core::uint32>(signalSemaphoreHandles.size());
  vkSumbitInfo.pSignalSemaphores     = signalSemaphoreHandles.data();

  VkResult result = vkQueueSubmit(m_handle, 1u, &vkSumbitInfo, fence.hasValue() ? fence.value()->getHandle() : VK_NULL_HANDLE);
  return result == VK_SUCCESS;
}

Queue_Present_Result Queue::present(const core::span<core::shared_ptr<Swap_Chain>>&  swapchains,
                                    const core::span<core::uint32>&                  imageIndices,
                                    const core::span<core::shared_ptr<Semaphore>>&   waitSemaphores)
{
  core::vector<VkSemaphore> waitSemaphoreHandles(waitSemaphores.size());
  core::Algorithms::copyc(waitSemaphores.begin(), waitSemaphores.end(), waitSemaphoreHandles.begin(), [](auto& src, auto& dst) -> void {
    *dst = (*src)->getHandle();
    });

  core::vector<VkSwapchainKHR> swapchainHandles(swapchains.size());
  core::Algorithms::copyc(swapchains.begin(), swapchains.end(), swapchainHandles.begin(), [](auto& src, auto& dst) -> void {
    *dst = (*src)->getHandle();
    });

  VkPresentInfoKHR vkPresentInfo{};
  vkPresentInfo.sType               = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  vkPresentInfo.pNext               = nullptr;
  vkPresentInfo.waitSemaphoreCount  = static_cast<core::uint32>(waitSemaphoreHandles.size());
  vkPresentInfo.pWaitSemaphores     = waitSemaphoreHandles.data();
  vkPresentInfo.swapchainCount      = static_cast<core::uint32>(swapchainHandles.size());
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
