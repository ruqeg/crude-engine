module;

#include <vulkan/vulkan.hpp>
#include <DirectXMath.h>
#include <flecs.h>

export module crude.graphics.renderer_frame_system;

export import crude.core.std_containers_stack;
export import crude.core.std_containers_heap;
export import crude.graphics.camera_ubo;
export import crude.graphics.renderer_core_system;

export namespace crude::graphics
{

template<class T>
class Uniform_Buffer;
class Command_Buffer;
class Semaphore;
class Fence;
class Mesh_Buffer;

struct Per_Frame
{
  Camera_UBO camera;
};

constexpr core::uint32 cFramesCount = 2u;

struct Renderer_Frame_System_Ctx
{
public:
  explicit Renderer_Frame_System_Ctx(core::shared_ptr<Renderer_Core_System_Ctx> coreCtx);
public:
  void stepToNextFrame();
  core::shared_ptr<Uniform_Buffer<Per_Frame>> getFramePerFrameUniformBuffer();
  core::shared_ptr<Command_Buffer> getFrameGraphicsCommandBuffer();
  core::shared_ptr<Semaphore> getFrameImageAvailableSemaphore();
  core::shared_ptr<Semaphore> getFrameRenderFinishedSemaphore();
  core::shared_ptr<Fence> getFrameInFlightFence();
public:
  core::shared_ptr<Renderer_Core_System_Ctx>                                  coreCtx;
  core::array<core::shared_ptr<Uniform_Buffer<Per_Frame>>, cFramesCount>      perFrameUniformBuffer;
  core::array<core::shared_ptr<Command_Buffer>, cFramesCount>                 graphicsCommandBuffers;
  core::array<core::shared_ptr<Semaphore>, cFramesCount>                      imageAvailableSemaphores;
  core::array<core::shared_ptr<Semaphore>, cFramesCount>                      renderFinishedSemaphores;
  core::array<core::shared_ptr<Fence>, cFramesCount>                          inFlightFences;
  core::uint32                                                                currentFrame;
  flecs::entity                                                               cameraNode; // !TODO move
  core::uint32                                                                swapchainImageIndex;
};

void rendererFrameStartSystemProcess(flecs::iter& it);
void rendererFrameSubmitSystemProcess(flecs::iter& it);

}