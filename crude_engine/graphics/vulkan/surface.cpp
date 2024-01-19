#include <graphics/vulkan/surface.hpp>
#include <graphics/vulkan/instance.hpp>

namespace crude_engine 
{

Surface::Surface(Shared_Ptr<const Instance> instance)
  :
  m_instance(instance)
{}

Surface::~Surface()
{
  vkDestroySurfaceKHR(CRUDE_OBJECT_HANDLE(m_instance), m_handle, getPVkAllocationCallbacks());
}

}
