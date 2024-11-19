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
export import crude.scene.light;
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
class Image_Cube;
class Image_View;
class Depth_Stencil_Cube_Attachment;

const Storage_Buffer_Descriptor cPointLightsBufferDescriptor{ 6u, VK_SHADER_STAGE_TASK_BIT_EXT };

struct Renderer_Point_Shadow_Pass_Systen_Ctx
{
public:
  explicit Renderer_Point_Shadow_Pass_Systen_Ctx(core::shared_ptr<Renderer_Frame_System_Ctx> frameCtx);
  void update(const core::vector<scene::Point_Light_GPU>& visiblePointLightsGPU);
private:
  void initializeRenderPass();
  void initalizeGraphicsPipeline();
  void initializeFramebuffers();
  core::shared_ptr<Descriptor_Set_Layout> createDescriptorSetLayout();
public:
  core::shared_ptr<Image_View>                               pointShadowImageView;
  core::vector<core::shared_ptr<Framebuffer>>                framebuffers;
  core::shared_ptr<Renderer_Frame_System_Ctx>                frameCtx;
  core::shared_ptr<Render_Pass>                              renderPass;
  core::shared_ptr<Pipeline>                                 pipeline;
  core::shared_ptr<graphics::Storage_Buffer>                 pointLightsShadowsBuffer;
  core::uint32                                               pointLightsShadowsCount;

  core::array<Uniform_Buffer_Descriptor, cFramesCount>       perFrameBufferDescriptors;
  graphics::Storage_Buffer_Descriptor                        submeshesDrawsBufferDescriptor;
  graphics::Storage_Buffer_Descriptor                        vertexBufferDescriptor;
  graphics::Storage_Buffer_Descriptor                        meshletBufferDescriptor;
  graphics::Storage_Buffer_Descriptor                        primitiveIndicesBufferDescriptor;
  graphics::Storage_Buffer_Descriptor                        vertexIndicesBufferDescriptor;
  graphics::Storage_Buffer_Descriptor                        pointLightsBufferDescriptor;
private:
  const VkFormat                                             depthStencilFormat;
  const core::uint32                                         dimensionSize;
  const VkSampleCountFlagBits                                samples;
private:
  static const core::array<Descriptor_Set_Layout_Binding, 7u>& getDescriptorLayoutBindings()
  {
    static const core::array<Descriptor_Set_Layout_Binding, 7u> descriptorLayoutBindings =
    {
      cPerFrameUniformBufferDescriptor,
      cSubmeshesDrawsBufferDescriptor,
      cVertexBufferDescriptor,
      cMeshletBufferDescriptor,
      cPrimitiveIndicesBufferDescriptor,
      cVertexIndicesBufferDescriptor,
      Storage_Buffer_Descriptor{ 6u, VK_SHADER_STAGE_TASK_BIT_EXT }
    };
    return descriptorLayoutBindings;
  };
  static const core::array<Descriptor_Pool_Size, 7u>& getDescriptorPoolSizes()
  {
    static const core::array<Descriptor_Pool_Size, 7u> descriptorPoolSizes =
    {
      Uniform_Buffer_Pool_Size(cFramesCount),
      Storage_Buffer_Pool_Size(1u),
      Storage_Buffer_Pool_Size(1u),
      Storage_Buffer_Pool_Size(1u),
      Storage_Buffer_Pool_Size(1u),
      Storage_Buffer_Pool_Size(1u),
      Storage_Buffer_Pool_Size(1u)
    };
    return descriptorPoolSizes;
  };
};

// 0 component - core::shared_ptr<Mesh_Buffer>
// 1 component - core::shared_ptr<scene::Mesh>
void rendererPointShadowPassSystemProcess(flecs::iter& it);

}