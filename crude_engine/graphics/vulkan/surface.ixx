module;

#include <vulkan/vulkan.hpp>
#include <SDL3/SDL_vulkan.h>

export module crude.gfx.vk.surface;

import crude.core.std_containers_stack;
import crude.core.std_containers_heap;
import crude.gfx.vk.instance;
import crude.gfx.vk.vulkan_object;
import crude.gfx.vk.vulkan_utils;
import crude.platform.sdl_window_container;

export namespace crude::gfx::vk
{

class Instance;

class Surface : public Vulkan_Object<VkSurfaceKHR>
{
public:
  explicit Surface(core::shared_ptr<const Instance> instance, core::shared_ptr<platform::SDL_Window_Container> window)
    :
    m_instance(instance),
    m_window(window)
  {
    const int result = SDL_Vulkan_CreateSurface(
      m_window->getHandle().get(),
      instance->getHandle(), 
      getPVkAllocationCallbacks(), 
      &m_handle);
    if (result != SDL_TRUE)
    {
      vulkanHandleError("Failed to create surface");
    }
  }
  ~Surface()
  {
    vkDestroySurfaceKHR(m_instance->getHandle(), m_handle, getPVkAllocationCallbacks());
  }

  static const core::span<const char* const> requiredExtensions()
  {
    Uint32 extensionsCount;
    const char* const* extensionsArray = SDL_Vulkan_GetInstanceExtensions(&extensionsCount);
    
    return core::span<const char* const>(extensionsArray, extensionsCount);
  }

protected:
  core::shared_ptr<const Instance>                m_instance;
  core::shared_ptr<platform::SDL_Window_Container>  m_window;
};

}
