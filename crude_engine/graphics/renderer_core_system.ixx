module;

export module crude.gfx.renderer_core_system;

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

struct Renderer_Core_System_Ctx
{
public:
  explicit Renderer_Core_System_Ctx(core::shared_ptr<platform::SDL_Window_Container> windowContainer);
private:
  void initializeInstance();
  void initializeSurface();
  void initializeDevice();
  void initializeSwapchain();
  void initalizeCommandPool();
  core::shared_ptr<vk::Physical_Device> pickPhysicalDevice();
  void initializeLogicDevice(core::shared_ptr<const vk::Physical_Device> physicalDevice);
public:
  core::shared_ptr<vk::Queue>                           graphicsQueue;
  core::shared_ptr<vk::Queue>                           presentQueue;
  core::shared_ptr<vk::Queue>                           transferQueue;

  core::shared_ptr<vk::Command_Pool>                    graphicsCommandPool;
  core::shared_ptr<vk::Command_Pool>                    transferCommandPool;

  core::shared_ptr<vk::Instance>                        instance;
  core::shared_ptr<vk::Device>                          device;
  core::shared_ptr<vk::Surface>                         surface;
  core::shared_ptr<vk::Swap_Chain>                      swapchain;
  core::vector<core::shared_ptr<vk::Swap_Chain_Image>>  swapchainImages;
  core::vector<core::shared_ptr<vk::Image_View>>        swapchainImagesViews;
  core::shared_ptr<vk::Debug_Utils_Messenger>           debugUtilsMessenger;
  core::shared_ptr<platform::SDL_Window_Container>      windowContainer;
};

}