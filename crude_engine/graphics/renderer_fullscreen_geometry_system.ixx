module;

#include <vulkan/vulkan.hpp>
#include <directxmath/DirectXMath.h>
#include <flecs.h>

export module crude.graphics.renderer_fullscreen_geometry_system;

export import crude.core.std_containers_stack;
export import crude.core.std_containers_heap;
export import crude.graphics.buffer_descriptor;
export import crude.graphics.descriptor_pool_size;
export import crude.graphics.image_descriptor;
import crude.graphics.renderer_frame_system;

export namespace crude::graphics
{

//class Render_Pass;
//class Command_Buffer;
//class Framebuffer;
//class Device;
//class Pipeline;
//class Depth_Stencil_Attachment;
//class Color_Attachment;
//class Descriptor_Pool;
//class Descriptor_Set_Layout;
//class Swap_Chain;
//class Semaphore;
//class Fence;
//class Mesh_Buffer;
//class Swap_Chain_Image;
//
//struct GBuffer
//{
//  core::uint32                               swapchainImagesCount;
//  VkExtent2D                                 extent;
//  core::shared_ptr<Color_Attachment>         albedoAttachment;
//  core::shared_ptr<Depth_Stencil_Attachment> depthStencilAttachment;
//};
//
//struct Renderer_Fullscreen_Geometry_Component
//{
//public:
//  Renderer_Fullscreen_Geometry_Component();
//  Renderer_Fullscreen_Geometry_Component(const GBuffer& gbuffer, core::shared_ptr<Color_Attachment> destinationColorAttachment);
//private:
//  core::shared_ptr<Descriptor_Set_Layout> createDescriptorSetLayout(core::shared_ptr<const Device> device);
//  void initializeRenderPass(core::shared_ptr<const Device> device, const GBuffer& gbuffer);
//  void initalizeGraphicsPipeline(core::shared_ptr<const Device> device);
//  void initializeFramebuffers(core::shared_ptr<Swap_Chain> swapchain, const GBuffer& gbuffer);
//public:
//  core::shared_ptr<Render_Pass>                renderPass;
//  core::shared_ptr<Pipeline>                   pipeline;
//  core::vector<core::shared_ptr<Framebuffer>>  framebuffers;
//};
//
//// 0 component - core::shared_ptr<Mesh_Buffer>
//// 1 component - core::shared_ptr<scene::Mesh>
//void rendererFullscreenGeometrySystemProcess(flecs::iter& it);

}