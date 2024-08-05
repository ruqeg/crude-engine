module;

#include <vulkan/vulkan.hpp>
#include <directxmath/DirectXMath.h>
#include <flecs.h>

export module crude.graphics.renderer_geometry_system;

export import crude.core.std_containers_stack;
export import crude.core.std_containers_heap;
export import crude.graphics.buffer_descriptor;
export import crude.graphics.descriptor_pool_size;
export import crude.graphics.image_descriptor;
import crude.graphics.renderer_frame_system;

export namespace crude::graphics
{

class Render_Pass;
class Command_Buffer;
class Framebuffer;
class Device;
class Pipeline;
class Depth_Stencil_Attachment;
class Color_Attachment;
class Descriptor_Pool;
class Descriptor_Set_Layout;
class Swap_Chain;
class Semaphore;
class Fence;
class Mesh_Buffer;
class Swap_Chain_Image;

struct Renderer_Geometry_Component
{
public:
  Renderer_Geometry_Component();
  Renderer_Geometry_Component(core::shared_ptr<Device>                        device,
                              core::shared_ptr<Color_Attachment>              colorAttachment,
                              core::shared_ptr<Depth_Stencil_Attachment>      depthStencilAttachment,
                              core::shared_ptr<Swap_Chain>                    swapchain,
                              core::span<const core::shared_ptr<Image_View>>  swapchainImagesViews);
private:
  core::shared_ptr<Descriptor_Set_Layout> createDescriptorSetLayout(core::shared_ptr<Device> device);
  void initializeRenderPass(core::shared_ptr<Device> device, core::shared_ptr<Color_Attachment> colorAttachment, core::shared_ptr<Depth_Stencil_Attachment> depthStencilAttachment, core::shared_ptr<Swap_Chain> swapchain);
  void initalizeGraphicsPipeline(core::shared_ptr<Device> device);
  void initializeFramebuffers(core::shared_ptr<Device> device, core::shared_ptr<Color_Attachment> colorAttachment, core::shared_ptr<Depth_Stencil_Attachment> depthStencilAttachment, core::span<const core::shared_ptr<Image_View>> swapchainImagesViews);
public:
  core::shared_ptr<Render_Pass>                                 renderPass;
  core::shared_ptr<Pipeline>                                    pipeline;
  core::vector<core::shared_ptr<Framebuffer>>                   framebuffers;

  core::array<Uniform_Buffer_Descriptor, cFramesCount>          perFrameBufferDescriptors;
  core::array<Combined_Image_Sampler_Descriptor, cFramesCount>  textureSamplerDescriptors;
  graphics::Storage_Buffer_Descriptor                           submeshesDrawsBufferDescriptor;
  graphics::Storage_Buffer_Descriptor                           vertexBufferDescriptor;
  graphics::Storage_Buffer_Descriptor                           meshletBufferDescriptor;
  graphics::Storage_Buffer_Descriptor                           primitiveIndicesBufferDescriptor;
  graphics::Storage_Buffer_Descriptor                           vertexIndicesBufferDescriptor;
};

void rendererGeometrySystemProcess(flecs::iter& it);

}