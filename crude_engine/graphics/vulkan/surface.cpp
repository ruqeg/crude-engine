#include <vulkan/vulkan.hpp>

module crude_engine.graphics.vulkan.surface;

import crude_engine.graphics.vulkan.instance;

namespace crude_engine 
{

Surface::Surface(Shared_Ptr<const Instance> instance)
  :
  m_instance(instance)
{
  m_instance = instance;
}

Surface::~Surface()
{
  vkDestroySurfaceKHR(m_instance->getHandle(), m_handle, getPVkAllocationCallbacks());
}

}
