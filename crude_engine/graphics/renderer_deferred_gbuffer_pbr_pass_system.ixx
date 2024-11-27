module;

#include <vulkan/vulkan.hpp>
#include <DirectXMath.h>
#include <flecs.h>

export module crude.gfx.renderer_deferred_gbuffer_pbr_pass_system;

export import crude.core.std_containers_stack;
export import crude.core.std_containers_heap;
export import crude.gfx.vk.buffer_descriptor;
export import crude.gfx.vk.descriptor_pool_size;
export import crude.gfx.vk.image_descriptor;
export import crude.gfx.vk.subpass_description;
export import crude.gfx.vk.attachment_description;
export import crude.gfx.renderer_frame_system;
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
class Swap_Chain_Image;

}

export namespace crude::gfx
{

class Mesh_Buffer;
class GBuffer;

}

export namespace crude::gfx
{

const vk::Uniform_Buffer_Descriptor          cPerFrameUniformBufferDescriptor{ 0u, VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_FRAGMENT_BIT };
const vk::Storage_Buffer_Descriptor          cSubmeshesDrawsBufferDescriptor{ 1u, VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT };
const vk::Storage_Buffer_Descriptor          cVertexBufferDescriptor{ 3u, VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT };
const vk::Storage_Buffer_Descriptor          cMeshletBufferDescriptor{ 2u, VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT };
const vk::Storage_Buffer_Descriptor          cPrimitiveIndicesBufferDescriptor{ 4u, VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT };
const vk::Storage_Buffer_Descriptor          cVertexIndicesBufferDescriptor{ 5u, VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT };
const vk::Storage_Buffer_Descriptor          cPointLightsBufferDescriptor{ 6u, VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT };

const vk::Combined_Image_Sampler_Descriptor  cSubmeshAlbedoDescriptor(6u, VK_SHADER_STAGE_FRAGMENT_BIT);
const vk::Combined_Image_Sampler_Descriptor  cSubmeshMetallicRoughnessDescriptor(7u, VK_SHADER_STAGE_FRAGMENT_BIT);
const vk::Combined_Image_Sampler_Descriptor  cSubmeshNormalDescriptor(8u, VK_SHADER_STAGE_FRAGMENT_BIT);

struct Per_Mesh
{
  // !TODO Move
  DirectX::XMFLOAT4X4  modelToWorld;
  core::uint32         submeshIndex;
};

struct Renderer_Deferred_GBuffer_PBR_Pass_Systen_Ctx
{
public:
  explicit Renderer_Deferred_GBuffer_PBR_Pass_Systen_Ctx(core::shared_ptr<Renderer_Frame_System_Ctx> frameCtx);
private:
  void initializeRenderPass();
  void initalizeGraphicsPipeline();
  void initializeFramebuffers();
  core::shared_ptr<vk::Descriptor_Set_Layout> createDescriptorSetLayout();
  core::vector<vk::Attachment_Description> getAttachmentsDescriptions();
  core::vector<core::shared_ptr<vk::Image_View>> getFramebufferAttachments();
  core::array<vk::Subpass_Description, 1> getSubpassDescriptions();
  vk::Color_Blend_State_Create_Info createColorBlendStateCreateInfo();
  vk::Depth_Stencil_State_Create_Info createDepthStencilStateCreateInfo();
public:
  core::shared_ptr<Renderer_Frame_System_Ctx>                       frameCtx;
  core::shared_ptr<GBuffer>                                         gbuffer;
  core::shared_ptr<vk::Render_Pass>                                 renderPass;
  core::shared_ptr<vk::Pipeline>                                    pipeline;
  core::vector<core::shared_ptr<vk::Framebuffer>>                   framebuffers;
  core::array<vk::Uniform_Buffer_Descriptor, cFramesCount>          perFrameBufferDescriptors;
  core::array<vk::Combined_Image_Sampler_Descriptor, cFramesCount>  submeshAlbedoDescriptors;
  core::array<vk::Combined_Image_Sampler_Descriptor, cFramesCount>  submeshMetallicRoughnessDescriptors;
  core::array<vk::Combined_Image_Sampler_Descriptor, cFramesCount>  submeshNormalDescriptors;
  vk::Storage_Buffer_Descriptor                                     submeshesDrawsBufferDescriptor;
  vk::Storage_Buffer_Descriptor                                     vertexBufferDescriptor;
  vk::Storage_Buffer_Descriptor                                     meshletBufferDescriptor;
  vk::Storage_Buffer_Descriptor                                     primitiveIndicesBufferDescriptor;
  vk::Storage_Buffer_Descriptor                                     vertexIndicesBufferDescriptor;
private:
  static const core::array<vk::Descriptor_Set_Layout_Binding, 9u>& getDescriptorLayoutBindings()
  {
    static const core::array<vk::Descriptor_Set_Layout_Binding, 9u> descriptorLayoutBindings =
    {
      cPerFrameUniformBufferDescriptor,
      cSubmeshAlbedoDescriptor,
      cSubmeshMetallicRoughnessDescriptor,
      cSubmeshNormalDescriptor,
      cSubmeshesDrawsBufferDescriptor,
      cVertexBufferDescriptor,
      cMeshletBufferDescriptor,
      cPrimitiveIndicesBufferDescriptor,
      cVertexIndicesBufferDescriptor
    };
    return descriptorLayoutBindings;
  };
  static const core::array<vk::Descriptor_Pool_Size, 9u>& getDescriptorPoolSizes()
  {
    static const core::array<vk::Descriptor_Pool_Size, 9u> descriptorPoolSizes =
    {
      vk::Uniform_Buffer_Pool_Size(cFramesCount),
      vk::Combined_Image_Sampler_Pool_Size(cFramesCount),
      vk::Combined_Image_Sampler_Pool_Size(cFramesCount),
      vk::Combined_Image_Sampler_Pool_Size(cFramesCount),
      vk::Storage_Buffer_Pool_Size(1u),
      vk::Storage_Buffer_Pool_Size(1u),
      vk::Storage_Buffer_Pool_Size(1u),
      vk::Storage_Buffer_Pool_Size(1u),
      vk::Storage_Buffer_Pool_Size(1u)
    };
    return descriptorPoolSizes;
  };
};

struct Deferred_Node_Pipeline_PBR_Flag {};

// 0 component - core::shared_ptr<Mesh_Buffer>
// 1 component - core::shared_ptr<scene::Mesh>
// 0 with - Deferred_Node_Pipeline_PBR_Flag
void rendererDeferredGBufferPbrPassSystemProcess(flecs::iter& it);

}