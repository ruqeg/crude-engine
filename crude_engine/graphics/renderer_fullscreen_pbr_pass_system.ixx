module;

#include <vulkan/vulkan.hpp>
#include <DirectXMath.h>
#include <flecs.h>

export module crude.graphics.renderer_fullscreen_pbr_pass_system;

export import crude.core.std_containers_stack;
export import crude.core.std_containers_heap;
export import crude.graphics.buffer_descriptor;
export import crude.graphics.descriptor_pool_size;
export import crude.graphics.image_descriptor;
export import crude.graphics.subpass_description;
export import crude.graphics.attachment_description;
export import crude.graphics.image_attachment;
export import crude.graphics.renderer_frame_system;
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

struct Renderer_Fullscreen_PBR_Pass_Ctx
{
public:
  explicit Renderer_Fullscreen_PBR_Pass_Ctx(core::shared_ptr<Renderer_Frame_System_Ctx> frameCtx, core::shared_ptr<GBuffer> gbuffer);
private:
  core::shared_ptr<Descriptor_Set_Layout> createDescriptorSetLayout();
  void initializeRenderPass();
  void initalizeGraphicsPipeline();
  void initializeFramebuffers();
  core::array<Subpass_Description, 1> getSubpassDescriptions();
  core::vector<Attachment_Description> getAttachmentsDescriptions();
    Color_Blend_State_Create_Info createColorBlendStateCreateInfo();
public:
  core::shared_ptr<Renderer_Frame_System_Ctx>  frameCtx;
  core::shared_ptr<Sampler>                    sampler;
  core::shared_ptr<Render_Pass>                renderPass;
  core::shared_ptr<Pipeline>                   pipeline;
  core::vector<core::shared_ptr<Framebuffer>>  framebuffers;
  core::shared_ptr<GBuffer>                    gbuffer;
  core::shared_ptr<Color_Attachment>           colorAttachment;

  core::array<Combined_Image_Sampler_Descriptor, cFramesCount>  albedoTextureDescriptors;
  core::array<Combined_Image_Sampler_Descriptor, cFramesCount>  metallicRoughnessTextureDescriptors;
  core::array<Combined_Image_Sampler_Descriptor, cFramesCount>  normalTextureDescriptors;
  core::array<Combined_Image_Sampler_Descriptor, cFramesCount>  depthTextureDescriptors;
};

void rendererFullscreenPBRPassSystemProcess(flecs::iter& it);

}