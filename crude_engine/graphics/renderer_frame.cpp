#include <vulkan/vulkan.hpp>
#include <DirectXMath.h>
#include <functional>

module crude.gfx.renderer_frame;

import crude.gfx.vk.fence;
import crude.gfx.vk.swap_chain;
import crude.gfx.vk.uniform_buffer;
import crude.gfx.vk.semaphore;
import crude.gfx.vk.queue;
import crude.gfx.vk.device;
import crude.core.logger;

namespace crude::gfx
{

Renderer_Frame::Renderer_Frame(core::shared_ptr<Renderer_Core> core, core::uint32 framesCount)
  : m_core{ core }
  , m_framesCount{ framesCount }
  , m_currentFrame{ 0u }
{
  m_graphicsCommandBuffers.reserve(m_framesCount);
  m_imageAvailableSemaphores.reserve(m_framesCount);
  m_renderFinishedSemaphores.reserve(m_framesCount);
  m_inFlightFences.reserve(m_framesCount);

  for (core::uint32 i = 0; i < m_framesCount; ++i)
  {
    m_graphicsCommandBuffers.push_back(core::allocateShared<vk::Command_Buffer>(core->getGraphicsCommandPool(), VK_COMMAND_BUFFER_LEVEL_PRIMARY));
    m_imageAvailableSemaphores.push_back(core::allocateShared<vk::Semaphore>(core->getDevice()));
    m_renderFinishedSemaphores.push_back(core::allocateShared<vk::Semaphore>(core->getDevice()));
    m_inFlightFences.push_back(core::allocateShared<vk::Fence>(core->getDevice(), VK_FENCE_CREATE_SIGNALED_BIT));
  }
}

void Renderer_Frame::startFrame()
{
  getInFlightFence()->wait();

  const vk::Swap_Chain_Next_Image acquireNextImageResult = m_core->getSwapchain()->acquireNextImage(getImageAvailableSemaphore());

  if (acquireNextImageResult.outOfDate())
  {
    // !TODO
    return;
  }
  else if (acquireNextImageResult.failedAcquire())
  {
    core::logError(core::Debug::Channel::Graphics, "Failed to acquire swap chain image!");
  }

  m_swapchainImageIndex = acquireNextImageResult.getImageIndex().value();

  getInFlightFence()->reset();

  if (!getGraphicsCommandBuffer()->begin())
  {
    core::logError(core::Debug::Channel::Graphics, "Failed to begin recording command buffer!");
  }
}

void Renderer_Frame::endFrame()
{
  if (!getGraphicsCommandBuffer()->end())
  {
    core::logError(core::Debug::Channel::Graphics, "Failed to record command buffer!");
  }

  core::uint32 waitStageMasks[1] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
  const bool graphicsQueueSubmited = m_core->getGraphicsQueue()->sumbit(
    core::array{ getGraphicsCommandBuffer() },
    waitStageMasks,
    core::array{ getImageAvailableSemaphore() },
    core::array{ getRenderFinishedSemaphore() },
    getInFlightFence());

  if (!graphicsQueueSubmited)
  {
    core::logError(core::Debug::Channel::Graphics, "Failed to submit draw command buffer!");
  }

  const vk::Queue_Present_Result presentResult = m_core->getPresentQueue()->present(
    core::array{ m_core->getSwapchain() },
    core::array{ m_swapchainImageIndex },
    core::array{ getRenderFinishedSemaphore() });

  bool framebufferResized = false;
  if (presentResult.outOfDate() || presentResult.suboptimal() || framebufferResized)
  {
    // !TODO
  }
  else if (presentResult.failed())
  {
    core::logError(core::Debug::Channel::Graphics, "Failed to present swap chain image!");
  }

  m_currentFrame = (m_currentFrame + 1u) % m_framesCount;
}

core::shared_ptr<Renderer_Core> Renderer_Frame::getCore() { return m_core; }
core::shared_ptr<vk::Command_Buffer> Renderer_Frame::getGraphicsCommandBuffer() { return m_graphicsCommandBuffers[m_currentFrame]; }
core::uint32 Renderer_Frame::getSwapchainImageIndex() { return m_swapchainImageIndex; }
core::shared_ptr<vk::Semaphore> Renderer_Frame::getImageAvailableSemaphore() { return m_imageAvailableSemaphores[m_currentFrame]; }
core::shared_ptr<vk::Semaphore> Renderer_Frame::getRenderFinishedSemaphore() { return m_renderFinishedSemaphores[m_currentFrame]; }
core::shared_ptr<vk::Fence> Renderer_Frame::getInFlightFence() { return m_inFlightFences[m_currentFrame]; };

}