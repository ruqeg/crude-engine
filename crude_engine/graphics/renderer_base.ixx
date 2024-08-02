module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.renderer_base;

export import crude.core.std_containers_heap;
export import crude.core.std_containers_stack;

export namespace crude::platform
{

class SDL_Window_Container;

}

export namespace crude::graphics
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

}

export namespace crude::graphics
{

class Renderer_Base
{
public:
  Renderer_Base(core::shared_ptr<platform::SDL_Window_Container> windowContainer);
  ~Renderer_Base();
  core::shared_ptr<Queue> getGraphicsQueue();
  core::shared_ptr<Queue> getPresentQueue();
  core::shared_ptr<Queue> getTransferQueue();
  core::shared_ptr<Instance> getInstance();
  core::shared_ptr<Device> getDevice();
  core::shared_ptr<Surface> getSurface();
  core::shared_ptr<Swap_Chain> getSwapchain();
  const core::vector<core::shared_ptr<Swap_Chain_Image>>& getSwapchainImages();
  const core::vector<core::shared_ptr<Image_View>>& getSwapchainImagesViews();
  core::shared_ptr<Debug_Utils_Messenger> getDebugUtilsMessenger();
  core::shared_ptr<platform::SDL_Window_Container> getWindowContainer();
private:
  void initializeInstance();
  void initializeSurface();
  void initializeDevice();
  void initializeSwapchain();
  core::shared_ptr<Physical_Device> pickPhysicalDevice();
  void initializeLogicDevice(core::shared_ptr<const Physical_Device> physicalDevice);
private:
  core::shared_ptr<Queue>                           m_graphicsQueue;
  core::shared_ptr<Queue>                           m_presentQueue;
  core::shared_ptr<Queue>                           m_transferQueue;
  core::shared_ptr<Instance>                        m_instance;
  core::shared_ptr<Device>                          m_device;
  core::shared_ptr<Surface>                         m_surface;
  core::shared_ptr<Swap_Chain>                      m_swapchain;
  core::vector<core::shared_ptr<Swap_Chain_Image>>  m_swapchainImages;
  core::vector<core::shared_ptr<Image_View>>        m_swapchainImagesViews;
  core::shared_ptr<Debug_Utils_Messenger>           m_debugUtilsMessenger;
  core::shared_ptr<platform::SDL_Window_Container>  m_windowContainer;
};

}