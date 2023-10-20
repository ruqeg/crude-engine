#pragma once

#include "core.hpp"
#include "object.hpp"
#include "include_vulkan.hpp"
#include <memory>

namespace crude_vulkan_01
{

class Instance;

struct SurfaceCreateInfo
{
  std::shared_ptr<const Instance>  instance;
  explicit SurfaceCreateInfo(std::shared_ptr<const Instance>  instance);
};

class Surface : public TObject<VkSurfaceKHR>
{
public:
  explicit Surface(const SurfaceCreateInfo& createInfo);
  ~Surface();
protected:
  std::shared_ptr<const Instance> m_instance;
};

#ifdef VK_KHR_xcb_surface

struct XCBSurfaceCreateInfo
{
  std::shared_ptr<const Instance>  instance;
  xcb_connection_t*                pConnection;
  xcb_window_t                     window;
  VkXcbSurfaceCreateFlagsKHR       flags;

  explicit XCBSurfaceCreateInfo(std::shared_ptr<const Instance>  instance,
                                xcb_connection_t*                pConnection,
                                xcb_window_t                     window,
                                VkXcbSurfaceCreateFlagsKHR       flags = 0u)
    :
    instance(instance),
    pConnection(pConnection),
    window(window),
    flags(flags)
  {}
};

class XCB_Surface : public Surface
{
public:
  XCB_Surface(const XCBSurfaceCreateInfo& createInfoXCB)
  :
  Surface(SurfaceCreateInfo(createInfoXCB.instance)),
  m_window(createInfoXCB.window),
  m_pConnection(createInfoXCB.pConnection)
{
  VkXcbSurfaceCreateInfoKHR vkCreateInfo;
  vkCreateInfo.sType       = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
  vkCreateInfo.connection  = createInfoXCB.pConnection;
  vkCreateInfo.window      = createInfoXCB.window;
  vkCreateInfo.flags       = createInfoXCB.flags;
  vkCreateInfo.pNext       = nullptr;

  const VkResult result = vkCreateXcbSurfaceKHR(
    CRUDE_VULKAN_01_HANDLE(m_instance),
    &vkCreateInfo, 
    nullptr, 
    &m_handle); 

  CRUDE_VULKAN_01_HANDLE_RESULT(result, "failed to create xcb surface");
}
  
  const xcb_connection_t* getConnection() const
  {
    return m_pConnection;
  }

  xcb_window_t getWindow() const
  {
    return m_window;
  }
private:
  const xcb_connection_t* m_pConnection;
  const xcb_window_t      m_window;
};

#endif // VK_KHR_xcb_surface

}
