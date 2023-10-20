#include "surface.hpp"
#include "instance.hpp"

namespace crude_vulkan_01 
{
  
SurfaceCreateInfo::SurfaceCreateInfo(std::shared_ptr<const Instance>  instance)
  :
  instance(instance)
{}

Surface::Surface(const SurfaceCreateInfo& createInfo)
{
  m_instance = createInfo.instance;
}

Surface::~Surface()
{
  vkDestroySurfaceKHR(
   CRUDE_VULKAN_01_HANDLE(m_instance), 
   m_handle, 
   nullptr);
}

}
