#include "surface.hpp"
#include "instance.hpp"

namespace crude_vulkan_01 
{
  
Surface_Create_Info::Surface_Create_Info(std::shared_ptr<const Instance>  instance)
  :
  instance(instance)
{}

Surface::Surface(const Surface_Create_Info& createInfo)
{
  m_instance = createInfo.instance;
}

Surface::~Surface()
{
  vkDestroySurfaceKHR(CRUDE_VULKAN_01_HANDLE(m_instance), m_handle, nullptr);
}

}
