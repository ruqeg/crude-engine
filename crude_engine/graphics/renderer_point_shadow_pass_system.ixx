module;

#include <vulkan/vulkan.hpp>
#include <DirectXMath.h>
#include <flecs.h>

export module crude.gfx.renderer_point_shadow_pass_system;

export import crude.core.std_containers_stack;
export import crude.core.std_containers_heap;
export import crude.gfx.vk.buffer_descriptor;
export import crude.gfx.vk.descriptor_pool_size;
export import crude.gfx.vk.image_descriptor;
export import crude.gfx.vk.subpass_description;
export import crude.gfx.vk.attachment_description;
export import crude.gfx.renderer_frame_system;
export import crude.scene.light;
import crude.gfx.vk.color_blend_state_create_info;
import crude.gfx.vk.depth_stencil_state_create_info;
import crude.gfx.vk.depth_stencil_state_create_info;
import crude.gfx.renderer_deferred_gbuffer_pbr_pass_system;

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
class Image_Cube;
class Image_View;
class Depth_Stencil_Cube_Attachment;

}


export namespace crude::gfx
{

class Mesh_Buffer;
class GBuffer;

}

export namespace crude::gfx
{

struct Renderer_Point_Shadow_Pass_Systen_Ctx
{
public:
  explicit Renderer_Point_Shadow_Pass_Systen_Ctx(core::shared_ptr<Renderer_Frame_System_Ctx> frameCtx);
  void update(const core::vector<scene::Point_Light_GPU>& visiblePointLightsGPU);
private:
  void initializeRenderPass();
  void initalizeGraphicsPipeline();
  void initializeFramebuffers();
  core::shared_ptr<vk::Descriptor_Set_Layout> createDescriptorSetLayout();
public:
  core::shared_ptr<vk::Image_View>                          pointShadowImageView;
  core::vector<core::shared_ptr<vk::Framebuffer>>           framebuffers;
  core::shared_ptr<Renderer_Frame_System_Ctx>               frameCtx;
  core::shared_ptr<vk::Render_Pass>                         renderPass;
  core::shared_ptr<vk::Pipeline>                            pipeline;
  core::shared_ptr<gfx::vk::Storage_Buffer>                 pointLightsShadowsBuffer;
  core::uint32                                              pointLightsShadowsCount;

  core::array<vk::Uniform_Buffer_Descriptor, cFramesCount>  perFrameBufferDescriptors;
  gfx::vk::Storage_Buffer_Descriptor                        submeshesDrawsBufferDescriptor;
  gfx::vk::Storage_Buffer_Descriptor                        vertexBufferDescriptor;
  gfx::vk::Storage_Buffer_Descriptor                        meshletBufferDescriptor;
  gfx::vk::Storage_Buffer_Descriptor                        primitiveIndicesBufferDescriptor;
  gfx::vk::Storage_Buffer_Descriptor                        vertexIndicesBufferDescriptor;
  gfx::vk::Storage_Buffer_Descriptor                        pointLightsBufferDescriptor;
private:
  const VkFormat                                             depthStencilFormat;
  const core::uint32                                         dimensionSize;
  const VkSampleCountFlagBits                                samples;
private:
  core::array<vk::Descriptor_Set_Layout_Binding, 7u> getDescriptorLayoutBindings()
  {
    const vk::Storage_Buffer_Descriptor cPofs = cPointLightsBufferDescriptor;
    core::array<vk::Descriptor_Set_Layout_Binding, 7u> descriptorLayoutBindings =
    {
      cPerFrameUniformBufferDescriptor,
      cSubmeshesDrawsBufferDescriptor,
      cVertexBufferDescriptor,
      cMeshletBufferDescriptor,
      cPrimitiveIndicesBufferDescriptor,
      cVertexIndicesBufferDescriptor,
      cPointLightsBufferDescriptor
    };
    return descriptorLayoutBindings;
  };
  core::array<vk::Descriptor_Pool_Size, 7u> getDescriptorPoolSizes()
  {
    core::array<vk::Descriptor_Pool_Size, 7u> descriptorPoolSizes =
    {
      vk::Uniform_Buffer_Pool_Size(cFramesCount),
      vk::Storage_Buffer_Pool_Size(1u),
      vk::Storage_Buffer_Pool_Size(1u),
      vk::Storage_Buffer_Pool_Size(1u),
      vk::Storage_Buffer_Pool_Size(1u),
      vk::Storage_Buffer_Pool_Size(1u),
      vk::Storage_Buffer_Pool_Size(1u)
    };
    return descriptorPoolSizes;
  };
};

// 0 component - core::shared_ptr<Mesh_Buffer>
// 1 component - core::shared_ptr<scene::Mesh>
void rendererPointShadowPassSystemProcess(flecs::iter& it);

}