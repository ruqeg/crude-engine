module;

#include <vulkan/vulkan.hpp>
#include <SDL3/SDL_vulkan.h>

export module crude.graphics.surface;

import crude.core.std_containers_stack;
import crude.core.std_containers_heap;
import crude.graphics.instance;
import crude.graphics.vulkan_object;
import crude.graphics.vulkan_utils;
import crude.platform.sdl_window_container;

export namespace crude::graphics
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
    const SDL_bool result = SDL_Vulkan_CreateSurface(
      m_window->getWindow().get(), 
      instance->getHandle(), 
      getPVkAllocationCallbacks(), 
      &m_handle);
    vulkanHandleResult(result ? VK_SUCCESS : VK_ERROR_UNKNOWN, "failed to create surface");
    
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
