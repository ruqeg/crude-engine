module;

#include <vulkan/vulkan.hpp>

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
export import crude.system.sdl_window_container;
export import crude.graphics.render_pass;
export import crude.graphics.pipeline;
export import crude.graphics.shader_module;
export import crude.graphics.pipeline_layout;
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
export import crude.graphics.descriptor_pool;
export import crude.graphics.descriptor_set;
export import crude.graphics.model_buffer;
export import crude.graphics.image_attachment;
export import crude.graphics.uniform_buffer;
export import crude.core.filesystem;
export import crude.scene.camera;

export namespace crude::graphics
{

struct Uniform_Buffer_Object
{
  math::Float4x4 model;
  math::Float4x4 view;
  math::Float4x4 proj;
};

// !TODO :D
class Renderer
{
private:
  struct Queue_Family_Indices
  {
    core::Optional<core::uint32> graphicsFamily;
    core::Optional<core::uint32> presentFamily;
    core::Optional<core::uint32> transferFamily;

    bool isComplete() const
    {
      return graphicsFamily.hasValue() && presentFamily.hasValue(); //&& transferFamily.hasValue();
    }
  };
public:
  Renderer(core::shared_ptr<system::SDL_Window_Container> windowContainer);
  ~Renderer();
public:
  void drawFrame();
  scene::Camera& getCamera() { return m_camera; }
private:
  void initializeInstance();
  void initializeSurface();
  void initializeDevice();
  void initializeSwapchain();
  void initalizeDescriptorSet();
  void initalizeGraphicsPipeline();
  void initalizeCommandPool();
  void initializeDepthImage();
  void initializeSwapchainFramebuffers();
  void initializeTextureImage();
  void initializeModelBuffer();
  void initializeUniformBuffers();
  void initializeCommandBuffers();
  void initializeSyncObjects();
private:
  core::shared_ptr<Physical_Device> pickPhysicalDevice();
  Queue_Family_Indices findDeviceQueueFamilies(core::shared_ptr<const Physical_Device> physicalDevice);
  bool checkDeviceExtensionSupport(core::shared_ptr<const Physical_Device> physicalDevice);
  bool checkSurfaceSupport(core::shared_ptr<const Physical_Device> physicalDevice, core::shared_ptr<Surface> surface);
  void initializeLogicDevice(core::shared_ptr<const Physical_Device> physicalDevice);
  VkSurfaceFormatKHR chooseSwapSurfaceFormat(const crude::core::vector<VkSurfaceFormatKHR>& availableFormats);
  VkPresentModeKHR chooseSwapPresentMode(const crude::core::vector<VkPresentModeKHR>& availablePresentModes);
  VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
  VkFormat findDepthFormat();
  VkFormat findSupportedFormat(const core::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
  core::vector<char> readFile(const char* filename);
  core::shared_ptr<Render_Pass> initializeRenderPass();
  void recordCommandBuffer(core::shared_ptr<Command_Buffer> commandBuffer, core::uint32 imageIndex);
  void updateUniformBuffer(core::uint32 currentImage);
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
  core::shared_ptr<system::SDL_Window_Container>        m_windowContainer;
  core::shared_ptr<Pipeline>                            m_graphicsPipeline;
  core::shared_ptr<Command_Pool>                        m_graphicsCommandPool;
  core::shared_ptr<Command_Pool>                        m_transferCommandPool;
  core::shared_ptr<Depth_Stencil_Attachment>            m_depthStencilAttachment;
  core::shared_ptr<Image_View>                          m_depthImageView;
  core::shared_ptr<Model_Buffer>                        m_modelBuffer;
  core::array<Uniform_Buffer_Descriptor, cFramesCount>         m_uniformBufferDesc;
  core::array<core::shared_ptr<Descriptor_Set>, cFramesCount>  m_descriptorSets;
  core::array<core::shared_ptr<Uniform_Buffer<Uniform_Buffer_Object>>, cFramesCount>  m_uniformBuffer;
  core::array<core::shared_ptr<Command_Buffer>, cFramesCount>   m_graphicsCommandBuffers;
  core::array<core::shared_ptr<Semaphore>, cFramesCount>        m_imageAvailableSemaphores;
  core::array<core::shared_ptr<Semaphore>, cFramesCount>        m_renderFinishedSemaphores;
  core::array<core::shared_ptr<Fence>, cFramesCount>            m_inFlightFences;
  core::uint32                                                  m_currentFrame;
  scene::Camera                                                 m_camera;
};

}