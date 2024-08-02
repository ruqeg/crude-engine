module;

#include <vulkan/vulkan.hpp>
#include <directxmath/DirectXMath.h>
#include <flecs.h>

export module crude.graphics.renderer;

export import crude.core.alias;
export import crude.graphics.queue;
export import crude.graphics.device;
export import crude.graphics.swap_chain;
export import crude.graphics.physical_device;
export import crude.graphics.descriptor_set_layout;
export import crude.graphics.instance;
export import crude.graphics.surface;
export import crude.graphics.debug_utils_messenger;
export import crude.core.std_containers_stack;
export import crude.core.std_containers_heap;
export import crude.platform.sdl_window_container;
export import crude.graphics.render_pass;
export import crude.graphics.pipeline;
export import crude.graphics.shader_module;
export import crude.graphics.pipeline_layout;
export import crude.graphics.push_constant_range;
export import crude.graphics.command_pool;
export import crude.graphics.command_buffer;
export import crude.graphics.fence;
export import crude.graphics.semaphore;
export import crude.graphics.image_view;
export import crude.graphics.image;
export import crude.graphics.buffer;
export import crude.graphics.device_memory;
export import crude.graphics.framebuffer;
export import crude.graphics.swap_chain_image;
export import crude.graphics.buffer_descriptor;
export import crude.graphics.image_descriptor;
export import crude.graphics.descriptor_pool;
export import crude.graphics.image_descriptor;
export import crude.graphics.sampler;
export import crude.graphics.descriptor_set;
export import crude.graphics.descriptor_set_layout;
export import crude.graphics.image_attachment;
export import crude.graphics.uniform_buffer;
export import crude.graphics.storage_buffer;
export import crude.graphics.camera_ubo;
export import crude.core.filesystem;

export namespace crude::graphics
{

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

class Renderer
{
public:
  Renderer(core::shared_ptr<platform::SDL_Window_Container> windowContainer, flecs::world world);
  ~Renderer();
public:
  void drawFrame();
  flecs::entity_view getNode() { return m_node; }
private:
  void initializeInstance();
  void initializeSurface();
  void initializeDevice();
  void initializeSwapchain();
  void initalizeDescriptorSet();
  void initalizeGraphicsPipeline();
  void initalizeCommandPool();
  void initializeColorResources();
  void initializeDepthImage();
  void initializeSwapchainFramebuffers();
  void initializeUniformBuffers();
  void initializeCommandBuffers();
  void initializeSyncObjects();
private:
  core::shared_ptr<Physical_Device> pickPhysicalDevice();
  void initializeLogicDevice(core::shared_ptr<const Physical_Device> physicalDevice);
  core::shared_ptr<Render_Pass> initializeRenderPass();
  void recordCommandBuffer(core::shared_ptr<Command_Buffer> commandBuffer, core::uint32 imageIndex);
private:
  static constexpr core::uint32 cFramesCount = 2u;
private:
  core::shared_ptr<Queue>                               m_graphicsQueue;
  core::shared_ptr<Queue>                               m_presentQueue;
  core::shared_ptr<Queue>                               m_transferQueue;
  core::shared_ptr<Instance>                            m_instance;
  core::shared_ptr<Device>                              m_device;
  core::shared_ptr<Surface>                             m_surface;
  core::shared_ptr<Swap_Chain>                          m_swapchain; 
  core::vector<core::shared_ptr<Swap_Chain_Image>>      m_swapchainImages;
  core::vector<core::shared_ptr<Image_View>>            m_swapchainImagesViews;
  core::vector<core::shared_ptr<Framebuffer>>           m_swapchainFramebuffers;
  core::shared_ptr<Debug_Utils_Messenger>               m_debugUtilsMessenger;
  core::shared_ptr<platform::SDL_Window_Container>      m_windowContainer;
  core::shared_ptr<Pipeline>                            m_graphicsPipeline;
  core::shared_ptr<Command_Pool>                        m_graphicsCommandPool;
  core::shared_ptr<Command_Pool>                        m_transferCommandPool;
  core::shared_ptr<Depth_Stencil_Attachment>            m_depthStencilAttachment;
  core::shared_ptr<Image_View>                          m_depthImageView;
  core::shared_ptr<Color_Attachment>                    m_colorAttachment;
  core::shared_ptr<Image_View>                          m_colorAttachmentView;
  graphics::Storage_Buffer_Descriptor                   m_submeshesDrawsBufferDescriptor;
  graphics::Storage_Buffer_Descriptor                   m_vertexBufferDescriptor;
  graphics::Storage_Buffer_Descriptor                   m_meshletBufferDescriptor;
  graphics::Storage_Buffer_Descriptor                   m_primitiveIndicesBufferDescriptor;
  graphics::Storage_Buffer_Descriptor                   m_vertexIndicesBufferDescriptor;
  core::shared_ptr<graphics::Descriptor_Set_Layout>     m_descriptorSetLayout;
  core::array<Uniform_Buffer_Descriptor, cFramesCount>         m_perFrameUniformBufferDesc;
  core::array<Combined_Image_Sampler_Descriptor, cFramesCount> m_textureSamplerDesc;
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