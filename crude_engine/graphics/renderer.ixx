module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.renderer;

export import crude.core.alias;
export import crude.core.shared_ptr;
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
export import crude.core.filesystem;
export import crude.scene.camera;

export namespace crude::graphics
{

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
  Renderer(core::Shared_Ptr<system::SDL_Window_Container> windowContainer);
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
  void initializeVertexBuffer();
  void initializeIndexBuffer();
  void initializeUniformBuffers();
  void initializeCommandBuffers();
  void initializeSyncObjects();
private:
  core::Shared_Ptr<Physical_Device> pickPhysicalDevice();
  Queue_Family_Indices findDeviceQueueFamilies(core::Shared_Ptr<Physical_Device> physicalDevice);
  bool checkDeviceExtensionSupport(core::Shared_Ptr<Physical_Device> physicalDevice);
  bool checkSurfaceSupport(core::Shared_Ptr<Physical_Device> physicalDevice, core::Shared_Ptr<Surface> surface);
  void initializeLogicDevice(core::Shared_Ptr<Physical_Device> physicalDevice);
  VkSurfaceFormatKHR chooseSwapSurfaceFormat(const crude::core::vector<VkSurfaceFormatKHR>& availableFormats);
  VkPresentModeKHR chooseSwapPresentMode(const crude::core::vector<VkPresentModeKHR>& availablePresentModes);
  VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
  VkFormat findDepthFormat();
  VkFormat findSupportedFormat(const core::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
  core::vector<char> readFile(const char* filename);
  core::Shared_Ptr<Render_Pass> initializeRenderPass();
  void recordCommandBuffer(core::Shared_Ptr<Command_Buffer> commandBuffer, core::uint32 imageIndex);
  void updateUniformBuffer(core::uint32 currentImage);
private:
  static constexpr core::uint32 cFramesCount = 2u;
private:
  core::Shared_Ptr<Instance>                            m_instance;
  core::Shared_Ptr<Device>                              m_device;
  core::Shared_Ptr<Queue>                               m_graphicsQueue;
  core::Shared_Ptr<Queue>                               m_presentQueue;
  core::Shared_Ptr<Queue>                               m_transferQueue;
  core::Shared_Ptr<Surface>                             m_surface;
  core::Shared_Ptr<Swap_Chain>                          m_swapchain; 
  core::vector<core::Shared_Ptr<Swap_Chain_Image>>      m_swapchainImages;
  core::vector<core::Shared_Ptr<Image_View>>            m_swapchainImagesViews;
  core::vector<core::Shared_Ptr<Framebuffer>>           m_swapchainFramebuffers;
  core::Shared_Ptr<Debug_Utils_Messenger>               m_debugUtilsMessenger;
  core::Shared_Ptr<system::SDL_Window_Container>        m_windowContainer;
  core::Shared_Ptr<Pipeline>                            m_graphicsPipeline;
  core::Shared_Ptr<Command_Pool>                        m_graphicsCommandPool;
  core::Shared_Ptr<Command_Pool>                        m_transferCommandPool;
  core::Shared_Ptr<Device_Memory>                       m_depthImageDeviceMemory;
  core::Shared_Ptr<Image>                               m_depthImage;
  core::Shared_Ptr<Image_View>                          m_depthImageView;
  core::Shared_Ptr<Buffer>                              m_vertexBuffer;
  core::Shared_Ptr<Device_Memory>                       m_vertexBufferMemory;
  core::Shared_Ptr<Buffer>                              m_indexBuffer;
  core::Shared_Ptr<Device_Memory>                       m_indexBufferMemory;
  core::array <core::Shared_Ptr<Descriptor_Set>, cFramesCount>  m_descriptorSets;
  core::array<core::Shared_Ptr<Buffer>, cFramesCount>           m_uniformBuffer;
  core::array<core::Shared_Ptr<Device_Memory>, cFramesCount>    m_uniformBufferMemory;
  core::array<core::Shared_Ptr<Command_Buffer>, cFramesCount>   m_graphicsCommandBuffers;
  core::array<core::Shared_Ptr<Semaphore>, cFramesCount>        m_imageAvailableSemaphores;
  core::array<core::Shared_Ptr<Semaphore>, cFramesCount>        m_renderFinishedSemaphores;
  core::array<core::Shared_Ptr<Fence>, cFramesCount>            m_inFlightFences;
  core::uint32                                                  m_currentFrame;
  scene::Camera                                                 m_camera;
};

}