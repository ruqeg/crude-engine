module;

#include <vulkan/vulkan.hpp>
#include <DirectXMath.h>
#include <flecs.h>

export module crude.graphics.fullscreen_pbr_pass_system;

export import crude.core.std_containers_stack;
export import crude.core.std_containers_heap;
export import crude.graphics.buffer_descriptor;
export import crude.graphics.descriptor_pool_size;
export import crude.graphics.image_descriptor;
export import crude.graphics.subpass_description;
export import crude.graphics.attachment_description;
export import crude.graphics.image_attachment;
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
class Image;
class Mesh_Buffer;
class Swap_Chain_Image;
class GBuffer;
class Sampler;
class Renderer_Core_Component;

struct Fullscreen_PBR_Pass_Component
{
  Fullscreen_PBR_Pass_Component();
  core::shared_ptr<Sampler>                    sampler;
  core::shared_ptr<Render_Pass>                renderPass;
  core::shared_ptr<Pipeline>                   pipeline;
  core::vector<core::shared_ptr<Framebuffer>>  framebuffers;
  core::shared_ptr<GBuffer>                    gbuffer;

  core::array<Combined_Image_Sampler_Descriptor, cFramesCount>  albedoTextureDescriptors;
  core::array<Combined_Image_Sampler_Descriptor, cFramesCount>  depthTextureDescriptors;
};

void fullscreenPBRPassComponentInitialize(flecs::iter& it);

void fullscreenPBRPassSystemProcess(flecs::iter& it);

}

namespace crude::graphics
{

core::shared_ptr<Descriptor_Set_Layout> createDescriptorSetLayout(Renderer_Core_Component* rendererCoreComponent);
void initializeRenderPass(Fullscreen_PBR_Pass_Component* fullscreenPbrPassComponent, Renderer_Core_Component* rendererCoreComponent);
void initalizeGraphicsPipeline(Fullscreen_PBR_Pass_Component* fullscreenPbrPassComponent, Renderer_Core_Component* rendererCoreComponent);
void initializeFramebuffers(Fullscreen_PBR_Pass_Component* fullscreenPbrPassComponent, Renderer_Core_Component* rendererCoreComponent);
core::array<Subpass_Description, 1> getSubpassDescriptions();
core::vector<Attachment_Description> getAttachmentsDescriptions(Renderer_Core_Component* rendererCoreComponent);
core::vector<core::shared_ptr<Image_View>> getFramebufferAttachments(core::shared_ptr<Image_View> swapchainImageView);
Color_Blend_State_Create_Info createColorBlendStateCreateInfo();

}