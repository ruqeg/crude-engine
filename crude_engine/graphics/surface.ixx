module;

#include <vulkan/vulkan.hpp>
#include <SDL3/SDL_vulkan.h>

export module crude.graphics.surface;

import crude.core.std_containers_stack;
import crude.core.shared_ptr;
import crude.graphics.instance;
import crude.graphics.vulkan_object;
import crude.graphics.vulkan_utils;
import crude.system.sdl_window_container;

export namespace crude::graphics
{

class Instance;

class Surface : public Vulkan_Object<VkSurfaceKHR>
{
public:
  explicit Surface(core::Shared_Ptr<const Instance> instance, core::Shared_Ptr<system::SDL_Window_Container> window)
    :
    m_instance(instance),
    m_window(window)
  {
    const SDL_bool result = SDL_Vulkan_CreateSurface(
      m_window->getWindow().get(), 
      instance->getHandle(), 
      getPVkAllocationCallbacks(), 
      &m_handle);
    vulkanHandleResult(result ? VK_SUCCESS : VK_ERROR_UNKNOWN, "failed to create win32 surface");
    
  }
  ~Surface()
  {
    vkDestroySurfaceKHR(m_instance->getHandle(), m_handle, getPVkAllocationCallbacks());
  }

  static const core::span<const char*> requiredExtensions()
  {
#ifdef _WIN32
    static const char* extensions[] = { "VK_KHR_win32_surface", "VK_KHR_surface" };
    return core::span<const char*>(extensions, 2u);
#endif
  }

protected:
  core::Shared_Ptr<const Instance>                m_instance;
  core::Shared_Ptr<system::SDL_Window_Container>  m_window;
};

}
