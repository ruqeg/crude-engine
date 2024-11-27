#include <vulkan/vulkan.hpp>
#include <DirectXMath.h>
#include <flecs.h>
#include <functional>

module crude.gfx.renderer_frame_system;

import crude.gfx.vk.fence;
import crude.gfx.vk.swap_chain;
import crude.gfx.vk.uniform_buffer;
import crude.gfx.vk.semaphore;
import crude.gfx.vk.queue;
import crude.gfx.vk.device;
import crude.core.logger;

namespace crude::gfx
{

void rendererFrameStartSystemProcess(flecs::iter& it)
{
  Renderer_Frame_System_Ctx* frameCtx = it.ctx<Renderer_Frame_System_Ctx>();
  core::shared_ptr<Renderer_Core_System_Ctx> coreCtx = frameCtx->coreCtx;

  frameCtx->getFrameInFlightFence()->wait();

  const vk::Swap_Chain_Next_Image acquireNextImageResult = coreCtx->swapchain->acquireNextImage(frameCtx->getFrameImageAvailableSemaphore());

  if (acquireNextImageResult.outOfDate())
  {
    // !TODO
    return;
  }
  else if (acquireNextImageResult.failedAcquire())
  {
    core::logError(core::Debug::Channel::Graphics, "Failed to acquire swap chain image!");
  }

  frameCtx->swapchainImageIndex = acquireNextImageResult.getImageIndex().value();

  Per_Frame* data = frameCtx->getFramePerFrameUniformBuffer()->mapUnsafe();
  data->camera = Camera_UBO(frameCtx->cameraNode);
  frameCtx->getFramePerFrameUniformBuffer()->unmap();

  frameCtx->getFrameInFlightFence()->reset();
  if (!frameCtx->getFrameGraphicsCommandBuffer()->begin())
  {
    core::logError(core::Debug::Channel::Graphics, "Failed to begin recording command buffer!");
  }
}

void rendererFrameSubmitSystemProcess(flecs::iter& it)
{
  Renderer_Frame_System_Ctx* frameCtx = it.ctx<Renderer_Frame_System_Ctx>();
  core::shared_ptr<Renderer_Core_System_Ctx> coreCtx = frameCtx->coreCtx;

  if (!frameCtx->getFrameGraphicsCommandBuffer()->end())
  {
    core::logError(core::Debug::Channel::Graphics, "Failed to record command buffer!");
  }

  core::uint32 waitStageMasks[1] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
  const bool graphicsQueueSubmited = coreCtx->graphicsQueue->sumbit(
    core::array{ frameCtx->getFrameGraphicsCommandBuffer() },
    waitStageMasks,
    core::array{ frameCtx->getFrameImageAvailableSemaphore() },
    core::array{ frameCtx->getFrameRenderFinishedSemaphore() },
    frameCtx->getFrameInFlightFence());

  if (!graphicsQueueSubmited)
  {
    core::logError(core::Debug::Channel::Graphics, "Failed to submit draw command buffer!");
  }

  const vk::Queue_Present_Result presentResult = coreCtx->presentQueue->present(
    core::array{ coreCtx->swapchain },
    core::array{ frameCtx->swapchainImageIndex },
    core::array{ frameCtx->getFrameRenderFinishedSemaphore() });

  bool framebufferResized = false;
  if (presentResult.outOfDate() || presentResult.suboptimal() || framebufferResized)
  {
    // !TODO
  }
  else if (presentResult.failed())
  {
    core::logError(core::Debug::Channel::Graphics, "Failed to present swap chain image!");
  }

  frameCtx->stepToNextFrame();
}

Renderer_Frame_System_Ctx::Renderer_Frame_System_Ctx(core::shared_ptr<Renderer_Core_System_Ctx> coreCtx)
  : coreCtx{ coreCtx }
{
  for (core::uint32 i = 0; i < cFramesCount; ++i)
  {
    perFrameUniformBuffer[i] = core::allocateShared<vk::Uniform_Buffer<Per_Frame>>(coreCtx->device);
  }
  for (core::uint32 i = 0; i < cFramesCount; ++i)
  {
    graphicsCommandBuffers[i] = core::allocateShared<vk::Command_Buffer>(coreCtx->graphicsCommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
  }

  for (core::uint32 i = 0; i < cFramesCount; i++)
  {
    imageAvailableSemaphores[i] = core::allocateShared<vk::Semaphore>(coreCtx->device);
    renderFinishedSemaphores[i] = core::allocateShared<vk::Semaphore>(coreCtx->device);
    inFlightFences[i] = core::allocateShared<vk::Fence>(coreCtx->device, VK_FENCE_CREATE_SIGNALED_BIT);
  }

  currentFrame = 0u;
}

void Renderer_Frame_System_Ctx::stepToNextFrame()
{
  currentFrame = (currentFrame + 1u) % cFramesCount;
}

core::shared_ptr<vk::Uniform_Buffer<Per_Frame>> Renderer_Frame_System_Ctx::getFramePerFrameUniformBuffer() { return perFrameUniformBuffer[currentFrame]; }
core::shared_ptr<vk::Command_Buffer> Renderer_Frame_System_Ctx::getFrameGraphicsCommandBuffer() { return graphicsCommandBuffers[currentFrame]; }
core::shared_ptr<vk::Semaphore> Renderer_Frame_System_Ctx::getFrameImageAvailableSemaphore() { return imageAvailableSemaphores[currentFrame]; }
core::shared_ptr<vk::Semaphore> Renderer_Frame_System_Ctx::getFrameRenderFinishedSemaphore() { return renderFinishedSemaphores[currentFrame]; }
core::shared_ptr<vk::Fence> Renderer_Frame_System_Ctx::getFrameInFlightFence() { return inFlightFences[currentFrame]; };

}