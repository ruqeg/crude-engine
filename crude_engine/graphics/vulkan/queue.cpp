#include <graphics/vulkan/queue.hpp>
#include <graphics/vulkan/command_buffer.hpp>
#include <graphics/vulkan/fence.hpp>
#include <graphics/vulkan/semaphore.hpp>
#include <graphics/vulkan/swap_chain.hpp>
#include <core/std_containers.hpp>

namespace crude_engine
{

Queue::Queue(uint32 familyIndex, uint32 index)
  :
  m_familyIndex(familyIndex),
  m_index(index)
{}

bool Queue::sumbit(const Array_Unsafe<Shared_Ptr<Command_Buffer>>&  commandBuffers,
                   const Array_Unsafe<VkPipelineStageFlags>&        waitStageMasks, 
                   const Array_Unsafe<Shared_Ptr<Semaphore>>&       waitSemaphores,
                   const Array_Unsafe<Shared_Ptr<Semaphore>>&       signalSemaphores,
                   const Optional<Shared_Ptr<Fence>>&               fence)
{
  if (waitSemaphores.size() != waitStageMasks.size())
  {
    return false;
  }

  vector<VkCommandBuffer> commandBuffersHandles(commandBuffers.size());
  Algorithms::copyc(commandBuffers.begin(), commandBuffers.end(), commandBuffersHandles.begin(), [](auto& src, auto& dst) -> void {
    *dst = CRUDE_OBJECT_HANDLE(*src);
  });

  vector<VkSemaphore> waitSemaphoreHandles(waitSemaphores.size());
  Algorithms::copyc(waitSemaphores.begin(), waitSemaphores.end(), waitSemaphoreHandles.begin(), [](auto& src, auto& dst) -> void {
    *dst = CRUDE_OBJECT_HANDLE(*src);
    });

  vector<VkSemaphore> signalSemaphoreHandles(signalSemaphores.size());
  Algorithms::copyc(signalSemaphores.begin(), signalSemaphores.end(), signalSemaphoreHandles.begin(), [](auto& src, auto& dst) -> void {
    *dst = CRUDE_OBJECT_HANDLE(*src);
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

  VkResult result = vkQueueSubmit(m_handle, 1u, &vkSumbitInfo, fence.hasValue() ? CRUDE_OBJECT_HANDLE(fence.value()) : VK_NULL_HANDLE);
  return result == VK_SUCCESS;
}

Queue_Present_Result Queue::present(const Array_Unsafe<Shared_Ptr<Swap_Chain>>&  swapchains,
                                    const Array_Unsafe<uint32>&                  imageIndices,
                                    const Array_Unsafe<Shared_Ptr<Semaphore>>&   waitSemaphores)
{
  vector<VkSemaphore> waitSemaphoreHandles(waitSemaphores.size());
  Algorithms::copyc(waitSemaphores.begin(), waitSemaphores.end(), waitSemaphoreHandles.begin(), [](auto& src, auto& dst) -> void {
    *dst = CRUDE_OBJECT_HANDLE(*src);
    });

  vector<VkSwapchainKHR> swapchainHandles(swapchains.size());
  Algorithms::copyc(swapchains.begin(), swapchains.end(), swapchainHandles.begin(), [](auto& src, auto& dst) -> void {
    *dst = CRUDE_OBJECT_HANDLE(*src);
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
