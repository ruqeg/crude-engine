module;

#include <vulkan/vulkan.hpp>
#include <SDL3/SDL_vulkan.h>

export module crude.graphics.surface;

import crude.core.std_containers_stack;
import crude.core.shared_ptr;
import crude.graphics.instance;
import crude.graphics.vulkan_object;
import crude.graphics.vulkan_utils;

export namespace crude::graphics
{

class Instance;

class Surface : public Vulkan_Object<VkSurfaceKHR>
{
public:
  explicit Surface(core::Shared_Ptr<const Instance> instance, SDL_Window* window)
    :
    m_instance(instance)
  {
    const VkResult result = SDL_Vulkan_CreateSurface(window, instance->getHandle(), getPVkAllocationCallbacks(), &m_handle)
      ? VK_SUCCESS : VK_ERROR_UNKNOWN;
    vulkanHandleResult(result, "failed to create win32 surface");
    
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
  core::Shared_Ptr<const Instance>  m_instance;
  const SDL_Window* m_window;
};

}
