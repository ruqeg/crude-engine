//!TODO make this code more readable 

module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.vulkan.surface;

import crude.core.std_containers_stack;
import crude.core.shared_ptr;
import crude.graphics.vulkan.instance;
import crude.graphics.vulkan.vulkan_object;
import crude.graphics.vulkan.vulkan_utils;

export namespace crude::graphics
{

class Instance;

class Surface : public Vulkan_Object<VkSurfaceKHR>
{
public:
  explicit Surface(core::Shared_Ptr<const Instance> instance);
  ~Surface();
protected:
  core::Shared_Ptr<const Instance>  m_instance;
};

}
