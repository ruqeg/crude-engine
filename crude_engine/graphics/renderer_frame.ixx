module;

#include <vulkan/vulkan.hpp>
#include <DirectXMath.h>

export module crude.gfx.renderer_frame;

export import crude.core.std_containers_stack;
export import crude.core.std_containers_heap;
export import crude.gfx.camera_ubo;
export import crude.gfx.renderer_core;

export namespace crude::gfx::vk
{

template<class T>
class Uniform_Buffer;
class Command_Buffer;
class Semaphore;
class Fence;

}

export namespace crude::gfx
{

struct Per_Frame
{
  Camera_UBO camera;
};

struct Renderer_Frame
{
public:
  explicit Renderer_Frame(core::shared_ptr<Renderer_Core> core, core::uint32 framesCount);
public:
  void startFrame();
  void endFrame();
  core::shared_ptr<vk::Command_Buffer> getGraphicsCommandBuffer();
private:
  core::shared_ptr<vk::Semaphore> getImageAvailableSemaphore();
  core::shared_ptr<vk::Semaphore> getRenderFinishedSemaphore();
  core::shared_ptr<vk::Fence> getInFlightFence();
private:
  core::shared_ptr<Renderer_Core>                     m_core;
  core::vector<core::shared_ptr<vk::Command_Buffer>>  m_graphicsCommandBuffers;
  core::vector<core::shared_ptr<vk::Semaphore>>       m_imageAvailableSemaphores;
  core::vector<core::shared_ptr<vk::Semaphore>>       m_renderFinishedSemaphores;
  core::vector<core::shared_ptr<vk::Fence>>           m_inFlightFences;
  core::uint32                                        m_currentFrame;
  core::uint32                                        m_swapchainImageIndex;
  const core::uint32                                  m_framesCount;
};

}