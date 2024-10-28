module;

#include <vulkan/vulkan.hpp>
#include <DirectXMath.h>
#include <flecs.h>

export module crude.graphics.renderer_point_shadow_pass_system;

export import crude.core.std_containers_stack;
export import crude.core.std_containers_heap;
export import crude.graphics.buffer_descriptor;
export import crude.graphics.descriptor_pool_size;
export import crude.graphics.image_descriptor;
export import crude.graphics.subpass_description;
export import crude.graphics.attachment_description;
export import crude.graphics.renderer_frame_system;
import crude.graphics.color_blend_state_create_info;
import crude.graphics.depth_stencil_state_create_info;
import crude.graphics.depth_stencil_state_create_info;
import crude.graphics.renderer_deferred_gbuffer_pbr_pass_system;

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

struct Renderer_Point_Shadow_Pass_Systen_Ctx
{
public:
  explicit Renderer_Point_Shadow_Pass_Systen_Ctx(core::shared_ptr<Renderer_Frame_System_Ctx> frameCtx);
  explicit Renderer_Point_Shadow_Pass_Systen_Ctx(core::shared_ptr<Renderer_Frame_System_Ctx> frameCtx, core::shared_ptr<GBuffer> gbuffer);
private:
  void initializeRenderPass();
  void initalizeGraphicsPipeline();
  void initializeFramebuffers();
  core::shared_ptr<Descriptor_Set_Layout> createDescriptorSetLayout();
  core::vector<Attachment_Description> getAttachmentsDescriptions();
  core::vector<core::shared_ptr<Image_View>> getFramebufferAttachments();
  core::array<Subpass_Description, 1> getSubpassDescriptions();
  Color_Blend_State_Create_Info createColorBlendStateCreateInfo();
  Depth_Stencil_State_Create_Info createDepthStencilStateCreateInfo();
public:
  core::shared_ptr<Renderer_Frame_System_Ctx>                   frameCtx;
  core::shared_ptr<Render_Pass>                                 renderPass;
  core::shared_ptr<Pipeline>                                    pipeline;
  core::vector<core::shared_ptr<Framebuffer>>                   framebuffers;
  core::shared_ptr<GBuffer>                                     gbuffer;
  core::array<Uniform_Buffer_Descriptor, cFramesCount>          perFrameBufferDescriptors;
  core::array<Combined_Image_Sampler_Descriptor, cFramesCount>  submeshMetallicRoughnessDescriptors;
  core::array<Combined_Image_Sampler_Descriptor, cFramesCount>  submeshNormalDescriptors;
  graphics::Storage_Buffer_Descriptor                           submeshesDrawsBufferDescriptor;
  graphics::Storage_Buffer_Descriptor                           vertexBufferDescriptor;
  graphics::Storage_Buffer_Descriptor                           meshletBufferDescriptor;
  graphics::Storage_Buffer_Descriptor                           primitiveIndicesBufferDescriptor;
  graphics::Storage_Buffer_Descriptor                           vertexIndicesBufferDescriptor;
private:
  static const core::array<Descriptor_Set_Layout_Binding, 6u>& getDescriptorLayoutBindings()
  {
    static const core::array<Descriptor_Set_Layout_Binding, 6u> descriptorLayoutBindings =
    {
      cPerFrameUniformBufferDescriptor,
      cSubmeshesDrawsBufferDescriptor,
      cVertexBufferDescriptor,
      cMeshletBufferDescriptor,
      cPrimitiveIndicesBufferDescriptor,
      cVertexIndicesBufferDescriptor
    };
    return descriptorLayoutBindings;
  };
  static const core::array<Descriptor_Pool_Size, 6u>& getDescriptorPoolSizes()
  {
    static const core::array<Descriptor_Pool_Size, 6u> descriptorPoolSizes =
    {
      Uniform_Buffer_Pool_Size(cFramesCount),
      Storage_Buffer_Pool_Size(1u),
      Storage_Buffer_Pool_Size(1u),
      Storage_Buffer_Pool_Size(1u),
      Storage_Buffer_Pool_Size(1u),
      Storage_Buffer_Pool_Size(1u)
    };
    return descriptorPoolSizes;
  };
};

struct Deferred_Node_Pipeline_Color_Flag {};

// 0 component - core::shared_ptr<Mesh_Buffer>
// 1 component - core::shared_ptr<scene::Mesh>
// 0 with - Deferred_Node_Pipeline_Color_Flag
void rendererDeferredGBufferColorPassSystemProcess(flecs::iter& it);

}