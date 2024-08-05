#include <vulkan/vulkan.hpp>
#include <directxmath/DirectXMath.h>
#include <flecs.h>
#include <functional>

module crude.graphics.renderer_frame_system;

import crude.graphics.fence;
import crude.graphics.swap_chain;
import crude.graphics.uniform_buffer;
import crude.graphics.semaphore;
import crude.graphics.renderer_core_component;
import crude.graphics.renderer_geometry_system;
import crude.graphics.queue;
import crude.graphics.device;
import crude.core.logger;

namespace crude::graphics
{

Renderer_Frame_Component::Renderer_Frame_Component(core::shared_ptr<Device> device, core::shared_ptr<Command_Pool> graphicsCommandPool)
{
  for (core::uint32 i = 0; i < cFramesCount; ++i)
  {
    perFrameUniformBuffer[i] = core::allocateShared<Uniform_Buffer<Per_Frame>>(device);
  }
  for (core::uint32 i = 0; i < cFramesCount; ++i)
  {
    graphicsCommandBuffers[i] = core::allocateShared<Command_Buffer>(graphicsCommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
  }

  for (core::uint32 i = 0; i < cFramesCount; i++)
  {
    imageAvailableSemaphores[i] = core::allocateShared<Semaphore>(device);
    renderFinishedSemaphores[i] = core::allocateShared<Semaphore>(device);
    inFlightFences[i] = core::allocateShared<Fence>(device, VK_FENCE_CREATE_SIGNALED_BIT);
  }

  currentFrame = 0u;
}

void Renderer_Frame_Component::stepToNextFrame()
{
  currentFrame = (currentFrame + 1u) % cFramesCount;
}

core::shared_ptr<Uniform_Buffer<Per_Frame>> Renderer_Frame_Component::getFramePerFrameUniformBuffer() { return perFrameUniformBuffer[currentFrame]; }
core::shared_ptr<Command_Buffer> Renderer_Frame_Component::getFrameGraphicsCommandBuffer() { return graphicsCommandBuffers[currentFrame]; }
core::shared_ptr<Semaphore> Renderer_Frame_Component::getFrameImageAvailableSemaphore() { return imageAvailableSemaphores[currentFrame]; }
core::shared_ptr<Semaphore> Renderer_Frame_Component::getFrameRenderFinishedSemaphore() { return renderFinishedSemaphores[currentFrame]; }
core::shared_ptr<Fence> Renderer_Frame_Component::getFrameInFlightFence() { return inFlightFences[currentFrame]; };

void renderSystemProcess(flecs::iter& it)
{
  Renderer_Core_Component* coreComponent = it.world().get_mut<Renderer_Core_Component>();
  Renderer_Frame_Component* fameComponent = it.world().get_mut<Renderer_Frame_Component>();
  Renderer_Geometry_Component* geometryComponent = it.world().get_mut<Renderer_Geometry_Component>();
  rendererComponent->getFrameInFlightFence()->wait();
  
  const Swap_Chain_Next_Image acquireNextImageResult = coreComponent->swapchain->acquireNextImage(rendererComponent->getFrameImageAvailableSemaphore());

  if (acquireNextImageResult.outOfDate())
  {
    // !TODO
    return;
  }
  else if (acquireNextImageResult.failedAcquire())
  {
    core::logError(core::Debug::Channel::Graphics, "Failed to acquire swap chain image!");
  }

  core::uint32 imageIndex = acquireNextImageResult.getImageIndex().value();

  rendererComponent->getFrameInFlightFence()->reset();
  if (!rendererComponent->getFrameGraphicsCommandBuffer()->begin())
  {
    core::logError(core::Debug::Channel::Graphics, "Failed to begin recording command buffer!");
  }
  Per_Frame* data = m_perFrameUniformBuffer[m_currentFrame]->mapUnsafe();
  data->camera = Camera_UBO(m_cameraNode);
  m_perFrameUniformBuffer[m_currentFrame]->unmap();

  geometryComponent->perFrameBufferDescriptors[rendererComponent->currentFrame].update();
  it.world().query().run(std::function(renderGeometrySystemProcess));

  if (!rendererComponent->getFrameGraphicsCommandBuffer()->end())
  {
    core::logError(core::Debug::Channel::Graphics, "Failed to record command buffer!");
  }

  core::uint32 waitStageMasks[1] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
  const bool graphicsQueueSubmited = coreComponent->graphicsQueue->sumbit(
    core::array{rendererComponent->getFrameGraphicsCommandBuffer()},
    waitStageMasks,
    core::array{rendererComponent->getFrameImageAvailableSemaphore()},
    core::array{rendererComponent->getFrameRenderFinishedSemaphore()},
    rendererComponent->getFrameInFlightFence());

  if (!graphicsQueueSubmited)
  {
    core::logError(core::Debug::Channel::Graphics, "Failed to submit draw command buffer!");
  }

  const Queue_Present_Result presentResult = coreComponent->presentQueue->present(
    core::array{ coreComponent->swapchain},
    core::array{ imageIndex },
    core::array{ rendererComponent->getFrameRenderFinishedSemaphore() });

  bool framebufferResized = false;
  if (presentResult.outOfDate() || presentResult.suboptimal() || framebufferResized)
  {
    // !TODO
  }
  else if (presentResult.failed())
  {
    core::logError(core::Debug::Channel::Graphics, "Failed to present swap chain image!");
  }

  rendererComponent->stepToNextFrame();
}

}