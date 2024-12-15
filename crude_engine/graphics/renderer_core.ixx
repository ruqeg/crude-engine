module;

export module crude.gfx.renderer_core;

export import crude.core.std_containers_heap;
export import crude.core.std_containers_stack;

export namespace crude::platform
{

class SDL_Window_Container;

}

export namespace crude::gfx::vk
{

class Physical_Device;
class Queue;
class Instance;
class Device;
class Surface;
class Swap_Chain;
class Swap_Chain_Image;
class Image_View;
class Debug_Utils_Messenger;
class Command_Pool;

}

export namespace crude::gfx
{

struct Renderer_Core
{
public:
  explicit Renderer_Core(core::shared_ptr<platform::SDL_Window_Container> windowContainer);
public:
  core::shared_ptr<vk::Queue> getGraphicsQueue();
  core::shared_ptr<vk::Queue> getPresentQueue();
  core::shared_ptr<vk::Queue> getTransferQueue();
  core::shared_ptr<vk::Command_Pool> getGraphicsCommandPool();
  core::shared_ptr<vk::Command_Pool> getTransferCommandPool();
  core::shared_ptr<vk::Instance> getInstance();
  core::shared_ptr<vk::Device> getDevice();
  core::shared_ptr<vk::Surface> getSurface();
  core::shared_ptr<vk::Swap_Chain> getSwapchain();
  const core::vector<core::shared_ptr<vk::Swap_Chain_Image>>& getSwapchainImages();
  const core::vector<core::shared_ptr<vk::Image_View>>& getSwapchainImagesViews();
  core::shared_ptr<vk::Debug_Utils_Messenger> getDebugUtilsMessenger();
  core::shared_ptr<platform::SDL_Window_Container> getWindowContainer();
private:
  void initializeInstance();
  void initializeSurface();
  void initializeDevice();
  void initializeSwapchain();
  void initalizeCommandPool();
  core::shared_ptr<vk::Physical_Device> pickPhysicalDevice();
  void initializeLogicDevice(core::shared_ptr<const vk::Physical_Device> physicalDevice);
public:
  core::shared_ptr<vk::Queue>                           m_graphicsQueue;
  core::shared_ptr<vk::Queue>                           m_presentQueue;
  core::shared_ptr<vk::Queue>                           m_transferQueue;
  core::shared_ptr<vk::Command_Pool>                    m_graphicsCommandPool;
  core::shared_ptr<vk::Command_Pool>                    m_transferCommandPool;
  core::shared_ptr<vk::Instance>                        m_instance;
  core::shared_ptr<vk::Device>                          m_device;
  core::shared_ptr<vk::Surface>                         m_surface;
  core::shared_ptr<vk::Swap_Chain>                      m_swapchain;
  core::vector<core::shared_ptr<vk::Swap_Chain_Image>>  m_swapchainImages;
  core::vector<core::shared_ptr<vk::Image_View>>        m_swapchainImagesViews;
  core::shared_ptr<vk::Debug_Utils_Messenger>           m_debugUtilsMessenger;
  core::shared_ptr<platform::SDL_Window_Container>      m_windowContainer;
};

}