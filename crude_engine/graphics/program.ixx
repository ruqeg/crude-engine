module;

#include <vulkan/vulkan.hpp>
#include <directxmath/DirectXMath.h>
#include <flecs.h>

export module crude.graphics.program;

export import crude.graphics.renderer_base;
export import crude.graphics.buffer_descriptor;
export import crude.graphics.camera_ubo;
export import crude.graphics.descriptor_pool_size;
export import crude.graphics.image_descriptor;

export namespace crude::graphics
{

constexpr core::uint32 cFramesCount = 2u;

constexpr core::uint32       cPerFrameUniformBufferBinding = 0u;
constexpr VkShaderStageFlags cPerFrameUniformBufferStageFlags = VK_SHADER_STAGE_MESH_BIT_EXT;

constexpr core::uint32       cSubmeshesDrawsBufferBinding = 1u;
constexpr VkShaderStageFlags cSubmeshesDrawsBufferStageFlags = VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT;

constexpr core::uint32       cMeshletBufferBinding = 2u;
constexpr VkShaderStageFlags cMeshletBufferStageFlags = VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT;

constexpr core::uint32       cVertexBufferBinding = 3u;
constexpr VkShaderStageFlags cVertexBufferStageFlags = VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT;

constexpr core::uint32       cPrimitiveIndicesBufferBinding = 4u;
constexpr VkShaderStageFlags cPrimitiveIndicesBufferStageFlags = VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT;

constexpr core::uint32       cVertexIndicesBufferBinding = 5u;
constexpr VkShaderStageFlags cVertexIndicesBufferStageFlags = VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT;

constexpr core::uint32       cTextureSamplerBinding = 6u;
constexpr VkShaderStageFlags cTextureSamplerStageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

const Uniform_Buffer_Descriptor         cPerFrameUniformBufferDescriptor(cPerFrameUniformBufferBinding, cPerFrameUniformBufferStageFlags);
const Combined_Image_Sampler_Descriptor cTextureSamplerDescriptor(cTextureSamplerBinding, cTextureSamplerStageFlags);

struct Program_Descriptors
{
  core::array<Uniform_Buffer_Descriptor, cFramesCount>         m_perFrameUniformBufferDescriptor { cPerFrameUniformBufferDescriptor, cPerFrameUniformBufferDescriptor };
  core::array<Combined_Image_Sampler_Descriptor, cFramesCount> m_textureSamplerDescriptor{ cTextureSamplerDescriptor, cTextureSamplerDescriptor };
  graphics::Storage_Buffer_Descriptor                          m_submeshesDrawsBufferDescriptor{ cSubmeshesDrawsBufferBinding, cSubmeshesDrawsBufferStageFlags };
  graphics::Storage_Buffer_Descriptor                          m_vertexBufferDescriptor{ cVertexBufferBinding, cVertexBufferStageFlags };
  graphics::Storage_Buffer_Descriptor                          m_meshletBufferDescriptor{ cMeshletBufferBinding, cMeshletBufferStageFlags };
  graphics::Storage_Buffer_Descriptor                          m_primitiveIndicesBufferDescriptor{ cPrimitiveIndicesBufferBinding, cPrimitiveIndicesBufferStageFlags };
  graphics::Storage_Buffer_Descriptor                          m_vertexIndicesBufferDescriptor{ cVertexIndicesBufferBinding, cVertexIndicesBufferStageFlags };

  core::array<Descriptor_Set_Layout_Binding, 7u> getDescriptorLayoutBindings()
  {
    return {
      m_perFrameUniformBufferDescriptor[0],
      m_textureSamplerDescriptor[0],
      m_submeshesDrawsBufferDescriptor,
      m_vertexBufferDescriptor,
      m_meshletBufferDescriptor,
      m_primitiveIndicesBufferDescriptor,
      m_vertexIndicesBufferDescriptor 
    };
  };

  core::array<Descriptor_Pool_Size, 7u> getDescriptorPoolSizes()
  {
    return {
      Uniform_Buffer_Pool_Size(cFramesCount),
      Combined_Image_Sampler_Pool_Size(cFramesCount),
      Storage_Buffer_Pool_Size(1u),
      Storage_Buffer_Pool_Size(1u),
      Storage_Buffer_Pool_Size(1u),
      Storage_Buffer_Pool_Size(1u),
      Storage_Buffer_Pool_Size(1u)
    };
  }
};

struct Per_Frame
{
  Camera_UBO camera;
};

struct Per_Mesh
{
  // !TODO Move
  DirectX::XMFLOAT4X4  modelToWorld;
  core::uint32         submeshIndex;
};

class Render_Pass;
class Command_Buffer;
class Framebuffer;
class Pipeline;
class Depth_Stencil_Attachment;
class Color_Attachment;
class Descriptor_Pool;
class Descriptor_Set_Layout;
class Semaphore;
class Fence;

class Program
{
public:
  Program(core::shared_ptr<Renderer_Base> rendererBase, flecs::world world);
  ~Program() = default;
public:
  void drawFrame();
  flecs::entity_view getNode() { return m_node; }
private:
  void initalizeDescriptorSet();
  void initalizeGraphicsPipeline();
  void initializeColorResources();
  void initializeDepthImage();
  void initializeSwapchainFramebuffers();
  void initializeUniformBuffers();
  void initializeCommandBuffers();
  void initializeSyncObjects();
private:
  core::shared_ptr<Render_Pass> initializeRenderPass();
  void recordCommandBuffer(core::shared_ptr<Command_Buffer> commandBuffer, core::uint32 imageIndex);
private:
  static constexpr core::uint32 cFramesCount = 2u;
private:
  core::vector<core::shared_ptr<Framebuffer>>           m_swapchainFramebuffers;
  core::shared_ptr<Pipeline>                            m_graphicsPipeline;
  core::shared_ptr<Depth_Stencil_Attachment>            m_depthStencilAttachment;
  core::shared_ptr<Image_View>                          m_depthImageView;
  core::shared_ptr<Color_Attachment>                    m_colorAttachment;
  core::shared_ptr<Image_View>                          m_colorAttachmentView;
  core::shared_ptr<Renderer_Base>                       m_rendererBase;

  Program_Descriptors                                   m_programDescriptors;

  core::shared_ptr<Descriptor_Set_Layout>     m_descriptorSetLayout;
  core::shared_ptr<Descriptor_Pool>           m_descriptorPool;

  core::array<core::shared_ptr<Uniform_Buffer<Per_Frame>>, cFramesCount>  m_perFrameUniformBuffer;

  core::array<core::shared_ptr<Command_Buffer>, cFramesCount>   m_graphicsCommandBuffers;

  core::array<core::shared_ptr<Semaphore>, cFramesCount>        m_imageAvailableSemaphores;
  core::array<core::shared_ptr<Semaphore>, cFramesCount>        m_renderFinishedSemaphores;
  core::array<core::shared_ptr<Fence>, cFramesCount>            m_inFlightFences;
  core::uint32                                                  m_currentFrame;
  flecs::entity                                                 m_node;
  flecs::entity                                                 m_cameraNode;
  flecs::world                                                  m_world;
};

}