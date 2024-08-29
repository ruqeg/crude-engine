module;

#include <flecs.h>
#include <vulkan/vulkan.hpp>

export module crude.graphics.renderer_core_system;

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

struct Renderer_Core_System_Ctx
{
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

void rendererCoreSystemInitialize(flecs::iter& it);
void rendererCoreSystemDeinitialize(flecs::iter& it);

}

namespace crude::graphics
{

void initializeInstance(Renderer_Core_System_Ctx* ctx);
void initializeSurface(Renderer_Core_System_Ctx* ctx);
void initializeDevice(Renderer_Core_System_Ctx* ctx);
void initializeSwapchain(Renderer_Core_System_Ctx* ctx);
void initalizeCommandPool(Renderer_Core_System_Ctx* ctx);
core::shared_ptr<Physical_Device> pickPhysicalDevice(Renderer_Core_System_Ctx* ctx);
void initializeLogicDevice(Renderer_Core_System_Ctx* ctx, core::shared_ptr<const Physical_Device> physicalDevice);

}