module;

#include <vulkan/vulkan.hpp>
#include <directxmath/DirectXMath.h>
#include <flecs.h>

export module crude.graphics.renderer_frame_system;

export import crude.core.std_containers_stack;
export import crude.core.std_containers_heap;
export import crude.graphics.camera_ubo;

export namespace crude::graphics
{

template<class T>
class Uniform_Buffer;
class Command_Buffer;
class Device;
class Semaphore;
class Fence;
class Command_Pool;

struct Per_Frame
{
  Camera_UBO camera;
};

constexpr core::uint32 cFramesCount = 2u;

struct Renderer_Frame_Component
{
public:
  Renderer_Frame_Component(core::shared_ptr<Device> device, core::shared_ptr<Command_Pool> graphicsCommandPool);
  ~Renderer_Frame_Component() = default;
public:
  void stepToNextFrame();
  core::shared_ptr<Uniform_Buffer<Per_Frame>> getFramePerFrameUniformBuffer();
  core::shared_ptr<Command_Buffer> getFrameGraphicsCommandBuffer();
  core::shared_ptr<Semaphore> getFrameImageAvailableSemaphore();
  core::shared_ptr<Semaphore> getFrameRenderFinishedSemaphore();
  core::shared_ptr<Fence> getFrameInFlightFence();
public:
  core::array<core::shared_ptr<Uniform_Buffer<Per_Frame>>, cFramesCount>  perFrameUniformBuffer;
  core::array<core::shared_ptr<Command_Buffer>, cFramesCount>             graphicsCommandBuffers;
  core::array<core::shared_ptr<Semaphore>, cFramesCount>                  imageAvailableSemaphores;
  core::array<core::shared_ptr<Semaphore>, cFramesCount>                  renderFinishedSemaphores;
  core::array<core::shared_ptr<Fence>, cFramesCount>                      inFlightFences;
  core::uint32                                                            currentFrame;
  flecs::entity                                                           cameraNode; // !TODO move
};

void renderFrameSystemProcess(flecs::iter& it);

}