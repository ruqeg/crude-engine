module;

#include <vulkan/vulkan.hpp>
#include <windows.h>

export module crude_engine.graphics.vulkan.surface;

import crude_engine.core.shared_ptr;
import crude_engine.graphics.vulkan.vulkan_object;

export namespace crude_engine
{

class Instance;

class Surface : public Vulkan_Object<VkSurfaceKHR>
{
public:
  explicit Surface(Shared_Ptr<const Instance> instance);
  ~Surface();
protected:
  Shared_Ptr<const Instance>  m_instance;
};

#ifdef VK_KHR_win32_surface

class Win32_Surface : public Surface
{
public:
  Win32_Surface(Shared_Ptr<const Instance>    instance,
                HINSTANCE                     hinstance,
                HWND                          hwnd,
                VkWin32SurfaceCreateFlagsKHR  flags = 0u)
    :
    Surface(instance),
    m_hwnd(hwnd),
    m_hinstance(hinstance)
  {
    VkWin32SurfaceCreateInfoKHR vkCreateInfo{};
    vkCreateInfo.sType      = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    vkCreateInfo.pNext      = nullptr;

    vkCreateInfo.hinstance  = hinstance;
    vkCreateInfo.hwnd       = hwnd;
    vkCreateInfo.flags      = flags;

    const VkResult result = vkCreateWin32SurfaceKHR(CRUDE_OBJECT_HANDLE(m_instance), &vkCreateInfo, getPVkAllocationCallbacks(), &m_handle);
    CRUDE_VULKAN_HANDLE_RESULT(result, "failed to create win32 surface");
  }

  HINSTANCE getHInstance() const
  {
    return m_hinstance;
  }

  HWND getHWND() const
  {
    return m_hwnd;
  }

  static const span<const char*> requiredExtensions()
  {
    static const char* extensions[] = {"VK_KHR_win32_surface", "VK_KHR_surface"};
    return span<const char*>(extensions, 2u);
  }
private:
  const HWND       m_hwnd;
  const HINSTANCE  m_hinstance;
};

#endif // VK_KHR_win32_surface

// !TODO fix
#ifdef VK_KHR_xcb_surface

struct XCB_Surface_CreateInfo : public Surface_Create_Info
{
  xcb_connection_t*                pConnection;
  xcb_window_t                     window;
  VkXcbSurfaceCreateFlagsKHR       flags;

  explicit XCB_Surface_CreateInfo(std::shared_ptr<const Instance>  instance,
                                  xcb_connection_t*                pConnection,
                                  xcb_window_t                     window,
                                  VkXcbSurfaceCreateFlagsKHR       flags = 0u)
    :
    Surface_Create_Info(instance),
    pConnection(pConnection),
    window(window),
    flags(flags)
  {}
};

class XCB_Surface : public Surface, public Requiring_Extensions
{
public:
  XCB_Surface(const XCB_Surface_CreateInfo& createInfoXCB)
    :
    Surface(createInfoXCB),
    m_window(createInfoXCB.window),
    m_pConnection(createInfoXCB.pConnection)
  {
    VkXcbSurfaceCreateInfoKHR vkCreateInfo;
    vkCreateInfo.sType       = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
    vkCreateInfo.pNext       = nullptr;

    vkCreateInfo.connection  = createInfoXCB.pConnection;
    vkCreateInfo.window      = createInfoXCB.window;
    vkCreateInfo.flags       = createInfoXCB.flags;

    const VkResult result = vkCreateXcbSurfaceKHR(CRUDE_VULKAN_01_HANDLE(m_instance), &vkCreateInfo, nullptr, &m_handle); 
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
  
  static const std::vector<const char*>& requiredExtensions()
  {
    static std::vector<const char*> extensions = {"VK_KHR_xcb_surface", "VK_KHR_surface"};
    return extensions;
  }
private:
  const xcb_connection_t*  m_pConnection;
  const xcb_window_t       m_window;
};

#endif // VK_KHR_xcb_surface

}
