module;

#include <vulkan/vulkan.hpp>
#include <DirectXMath.h>
#include <flecs.h>

export module crude.gfx.renderer_fullscreen_pbr_pass_system;

export import crude.core.std_containers_stack;
export import crude.core.std_containers_heap;
export import crude.gfx.vk.buffer_descriptor;
export import crude.gfx.vk.descriptor_pool_size;
export import crude.gfx.vk.image_descriptor;
export import crude.gfx.vk.subpass_description;
export import crude.gfx.vk.attachment_description;
export import crude.gfx.vk.image_attachment;
export import crude.gfx.renderer_frame_system;
export import crude.gfx.renderer_light_system;
import crude.gfx.vk.color_blend_state_create_info;
import crude.gfx.vk.depth_stencil_state_create_info;
import crude.gfx.vk.depth_stencil_state_create_info;

export namespace crude::gfx::vk
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
class Swap_Chain_Image;
class Sampler;

}

export namespace crude::gfx
{

class Mesh_Buffer;
class GBuffer;

}

export namespace crude::gfx
{

struct PBRDebug
{
  core::float32       ndfConstant;
  core::float32       gsConstant;
  core::int32         ndfIndex;
  core::int32         gsIndex;
  core::int32         fsIndex;
  DirectX::XMFLOAT3A  fsConstant;
  DirectX::XMFLOAT3A  diffCoeff;
  DirectX::XMFLOAT3A  specCoeff;
};

struct Renderer_Fullscreen_PBR_Pass_Ctx
{
public:
  explicit Renderer_Fullscreen_PBR_Pass_Ctx(core::shared_ptr<Renderer_Frame_System_Ctx> frameCtx, core::shared_ptr<Renderer_Light_Ctx> lightCtx, core::shared_ptr<GBuffer> gbuffer);
private:
  core::shared_ptr<vk::Descriptor_Set_Layout> createDescriptorSetLayout();
  void initializeRenderPass();
  void initalizeGraphicsPipeline();
  void initializeFramebuffers();
  core::array<vk::Subpass_Description, 1> getSubpassDescriptions();
  core::vector<vk::Attachment_Description> getAttachmentsDescriptions();
  vk::Color_Blend_State_Create_Info createColorBlendStateCreateInfo();
public:
  core::shared_ptr<Renderer_Light_Ctx>                              lightCtx;
  core::shared_ptr<Renderer_Frame_System_Ctx>                       frameCtx;
  core::shared_ptr<vk::Sampler>                                     sampler;
  core::shared_ptr<vk::Render_Pass>                                 renderPass;
  core::shared_ptr<vk::Pipeline>                                    pipeline;
  core::vector<core::shared_ptr<vk::Framebuffer>>                   framebuffers;
  core::shared_ptr<GBuffer>                                         gbuffer;
  core::shared_ptr<vk::Color_Attachment>                            colorAttachment;

  core::array<vk::Uniform_Buffer_Descriptor, cFramesCount>          perFrameBufferDescriptors;
  core::array<vk::Combined_Image_Sampler_Descriptor, cFramesCount>  albedoTextureDescriptors;
  core::array<vk::Combined_Image_Sampler_Descriptor, cFramesCount>  metallicRoughnessTextureDescriptors;
  core::array<vk::Combined_Image_Sampler_Descriptor, cFramesCount>  normalTextureDescriptors;
  core::array<vk::Combined_Image_Sampler_Descriptor, cFramesCount>  depthTextureDescriptors;
  gfx::vk::Storage_Buffer_Descriptor                                pointLightsBufferDescriptor;
  core::array<vk::Uniform_Buffer_Descriptor, cFramesCount>          pbrDebugBufferDescriptors;
  PBRDebug                                                          pbrDebug{};
  core::array<core::shared_ptr<gfx::vk::Uniform_Buffer<PBRDebug>>, cFramesCount>          pbrDebugBuffers;
};

void rendererFullscreenPBRPassSystemProcess(flecs::iter& it);

}