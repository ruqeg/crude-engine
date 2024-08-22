module;

#include <vulkan/vulkan.hpp>
#include <DirectXMath.h>
#include <flecs.h>

export module crude.graphics.deferred_gbuffer_pass_system;

export import crude.core.std_containers_stack;
export import crude.core.std_containers_heap;
export import crude.graphics.buffer_descriptor;
export import crude.graphics.descriptor_pool_size;
export import crude.graphics.image_descriptor;
export import crude.graphics.subpass_description;
export import crude.graphics.attachment_description;
import crude.graphics.renderer_frame_system;
import crude.graphics.color_blend_state_create_info;
import crude.graphics.depth_stencil_state_create_info;
import crude.graphics.depth_stencil_state_create_info;

export namespace crude::graphics
{

class Render_Pass;
class Command_Buffer;
class Framebuffer;
class Pipeline;
class Descriptor_Pool;
class Descriptor_Set_Layout;
class Swap_Chain;
class Semaphore;
class Fence;
class Device;
class Mesh_Buffer;
class Swap_Chain_Image;
class GBuffer;
class Renderer_Core_Component;

struct Deferred_GBuffer_Pass_Component
{
  Deferred_GBuffer_Pass_Component();
  core::shared_ptr<Render_Pass>                                 renderPass;
  core::shared_ptr<Pipeline>                                    pipeline;
  core::vector<core::shared_ptr<Framebuffer>>                   framebuffers;
  core::shared_ptr<GBuffer>                                     gbuffer;
  core::array<Uniform_Buffer_Descriptor, cFramesCount>          perFrameBufferDescriptors;
  core::array<Combined_Image_Sampler_Descriptor, cFramesCount>  submeshTextureDescriptors;
  graphics::Storage_Buffer_Descriptor                           submeshesDrawsBufferDescriptor;
  graphics::Storage_Buffer_Descriptor                           vertexBufferDescriptor;
  graphics::Storage_Buffer_Descriptor                           meshletBufferDescriptor;
  graphics::Storage_Buffer_Descriptor                           primitiveIndicesBufferDescriptor;
  graphics::Storage_Buffer_Descriptor                           vertexIndicesBufferDescriptor;
};

void deferredGBufferPassSystemComponentInitialize(flecs::iter& it);

// 0 component - core::shared_ptr<Mesh_Buffer>
// 1 component - core::shared_ptr<scene::Mesh>
void deferredGBufferPassSystemProcess(flecs::iter& it);

}

namespace crude::graphics
{

void initializeRenderPass(Deferred_GBuffer_Pass_Component* deferredGBufferComponent, Renderer_Core_Component* rendererCoreComponent);
void initalizeGraphicsPipeline(Deferred_GBuffer_Pass_Component* deferredGBufferComponent, Renderer_Core_Component* rendererCoreComponent);
void initializeFramebuffers(Deferred_GBuffer_Pass_Component* deferredGBufferComponent, Renderer_Core_Component* rendererCoreComponent);
core::shared_ptr<Descriptor_Set_Layout> createDescriptorSetLayout(Renderer_Core_Component* rendererCoreComponent);
core::vector<Attachment_Description> getAttachmentsDescriptions(Deferred_GBuffer_Pass_Component* deferredGBufferComponent);
core::vector<core::shared_ptr<Image_View>> getFramebufferAttachments(Deferred_GBuffer_Pass_Component* deferredGBufferComponent);
core::array<Subpass_Description, 1> getSubpassDescriptions();
Color_Blend_State_Create_Info createColorBlendStateCreateInfo();
Depth_Stencil_State_Create_Info createDepthStencilStateCreateInfo();

}