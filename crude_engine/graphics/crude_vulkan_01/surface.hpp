#pragma once

#include "../../core/core.hpp"
#include "object.hpp"
#include "include_vulkan.hpp"
#include "requiring_extensions.hpp"

namespace crude_vulkan_01
{

class Instance;

struct Surface_Create_Info
{
  std::shared_ptr<const Instance>  instance;
  explicit Surface_Create_Info(std::shared_ptr<const Instance>  instance);
};

class Surface : public TObject<VkSurfaceKHR>
{
public:
  explicit Surface(const Surface_Create_Info& createInfo);
  ~Surface();
protected:
  std::shared_ptr<const Instance> m_instance;
};

#ifdef VK_KHR_win32_surface

struct Win32_Surface_Create_Info : public Surface_Create_Info
{
  HINSTANCE                        hinstance;
  HWND                             hwnd;
  VkWin32SurfaceCreateFlagsKHR     flags;

  explicit Win32_Surface_Create_Info(std::shared_ptr<const Instance>  instance,
                                     HINSTANCE                        hinstance,
                                     HWND                             hwnd,
                                     VkWin32SurfaceCreateFlagsKHR     flags = 0u)
    :
    Surface_Create_Info(instance),
    hinstance(hinstance),
    hwnd(hwnd),
    flags(flags)
  {}
};

class Win32_Surface : public Surface, public Requiring_Extensions
{
public:
  Win32_Surface(const Win32_Surface_Create_Info& createInfoWin32)
    :
    Surface(createInfoWin32),
    m_hwnd(createInfoWin32.hwnd),
    m_hinstance(createInfoWin32.hinstance)
  {
    VkWin32SurfaceCreateInfoKHR vkCreateInfo{};
    vkCreateInfo.sType      = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    vkCreateInfo.pNext      = nullptr;

    vkCreateInfo.hinstance  = createInfoWin32.hinstance;
    vkCreateInfo.hwnd       = createInfoWin32.hwnd;
    vkCreateInfo.flags      = createInfoWin32.flags;

    const VkResult result = vkCreateWin32SurfaceKHR(CRUDE_VULKAN_01_HANDLE(m_instance), &vkCreateInfo, nullptr, &m_handle);
    CRUDE_VULKAN_01_HANDLE_RESULT(result, "failed to create win32 surface");
  }

  HINSTANCE getHInstance() const
  {
    return m_hinstance;
  }

  HWND getHWND() const
  {
    return m_hwnd;
  }

  static const std::vector<const char*>& requiredExtensions()
  {
    static std::vector<const char*> extensions = { "VK_KHR_win32_surface", "VK_KHR_surface" };
    return extensions;
  }
private:
  const HWND       m_hwnd;
  const HINSTANCE  m_hinstance;
};

#endif // VK_KHR_win32_surface

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
