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
export import crude.graphics.renderer_frame_component;

export namespace crude::scene
{

class Mesh;

}

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

struct Renderer_Geometry_Component
{
public:
  Renderer_Geometry_Component() = default;
  Renderer_Geometry_Component(core::shared_ptr<Device>                    device,
                              core::shared_ptr<Color_Attachment>          colorAttachment,
                              core::shared_ptr<Depth_Stencil_Attachment>  depthStencilAttachment);
public:
  core::shared_ptr<Render_Pass> getRenderPass();
  core::shared_ptr<Pipeline> getPipeline();
  core::vector<core::shared_ptr<Framebuffer>> getFramebuffers();
  core::array<Uniform_Buffer_Descriptor, cFramesCount> getPerFrameUniformBufferDescriptor();
  core::array<Combined_Image_Sampler_Descriptor, cFramesCount> getTextureSamplerDescriptor();
  graphics::Storage_Buffer_Descriptor getSubmeshesDrawsBufferDescriptor();
  graphics::Storage_Buffer_Descriptor getVertexBufferDescriptor();
  graphics::Storage_Buffer_Descriptor getMeshletBufferDescriptor();
  graphics::Storage_Buffer_Descriptor getPrimitiveIndicesBufferDescriptor();
  graphics::Storage_Buffer_Descriptor getVertexIndicesBufferDescriptor();
private:
  core::shared_ptr<Descriptor_Set_Layout> createDescriptorSetLayout(core::shared_ptr<Device> device);
  void initializeRenderPass(core::shared_ptr<Device> device, core::shared_ptr<Color_Attachment> colorAttachment, core::shared_ptr<Depth_Stencil_Attachment> depthStencilAttachment);
  void initalizeGraphicsPipeline(core::shared_ptr<Device> device);
  void initializeFramebuffers(core::shared_ptr<Device> device, core::shared_ptr<Color_Attachment> colorAttachment, core::shared_ptr<Depth_Stencil_Attachment> depthStencilAttachment);
private:
  core::shared_ptr<Render_Pass>                                 m_renderPass;
  core::shared_ptr<Pipeline>                                    m_pipeline;
  core::vector<core::shared_ptr<Framebuffer>>                   m_framebuffers;

  core::array<Uniform_Buffer_Descriptor, cFramesCount>          m_perFrameUniformBufferDescriptor;
  core::array<Combined_Image_Sampler_Descriptor, cFramesCount>  m_textureSamplerDescriptor;
  graphics::Storage_Buffer_Descriptor                           m_submeshesDrawsBufferDescriptor;
  graphics::Storage_Buffer_Descriptor                           m_vertexBufferDescriptor;
  graphics::Storage_Buffer_Descriptor                           m_meshletBufferDescriptor;
  graphics::Storage_Buffer_Descriptor                           m_primitiveIndicesBufferDescriptor;
  graphics::Storage_Buffer_Descriptor                           m_vertexIndicesBufferDescriptor;

};

void renderGeometrySystemProcess(flecs::iter& it);
void renderGeometrySystemEach(flecs::iter& it, size_t index, core::shared_ptr<scene::Mesh> mesh, core::shared_ptr<Mesh_Buffer> meshBuffer);

}