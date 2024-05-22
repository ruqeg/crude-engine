#include <vulkan/vulkan.hpp>

module crude.graphics.vulkan.surface;

import crude.graphics.vulkan.instance;

namespace crude::graphics
{

Surface::Surface(core::Shared_Ptr<const Instance> instance)
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
