module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.renderer_core_component;

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
class Command_Pool;

}

export namespace crude::graphics
{

class Renderer_Core_Component
{
public:
  Renderer_Core_Component() = default;
  Renderer_Core_Component(core::shared_ptr<platform::SDL_Window_Container> windowContainer);
  ~Renderer_Core_Component();
private:
  void initializeInstance();
  void initializeSurface();
  void initializeDevice();
  void initializeSwapchain();
  void initalizeCommandPool();
  core::shared_ptr<Physical_Device> pickPhysicalDevice();
  void initializeLogicDevice(core::shared_ptr<const Physical_Device> physicalDevice);
public:
  core::shared_ptr<Queue>                           graphicsQueue;
  core::shared_ptr<Queue>                           presentQueue;
  core::shared_ptr<Queue>                           transferQueue;

  core::shared_ptr<Command_Pool>                    graphicsCommandPool;
  core::shared_ptr<Command_Pool>                    transferCommandPool;

  core::shared_ptr<Instance>                        instance;
  core::shared_ptr<Device>                          device;
  core::shared_ptr<Surface>                         surface;
  core::shared_ptr<Swap_Chain>                      swapchain;
  core::vector<core::shared_ptr<Swap_Chain_Image>>  swapchainImages;
  core::vector<core::shared_ptr<Image_View>>        swapchainImagesViews;
  core::shared_ptr<Debug_Utils_Messenger>           debugUtilsMessenger;
  core::shared_ptr<platform::SDL_Window_Container>  windowContainer;
};

}