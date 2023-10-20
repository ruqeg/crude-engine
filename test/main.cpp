#include <xcb/xcb.h>

#define VK_USE_PLATFORM_XCB_KHR
#include "../include/crude_vulkan_01/instance.hpp"
#include "../include/crude_vulkan_01/debug_utils_messenger.hpp"
#include "../include/crude_vulkan_01/surface.hpp"

#include <memory>
#include <iostream>

class Test_Application
{
public:
  void run()
  {
    initWindow();
    initVulkan();
    cleanup();
  }
private:
  void initWindow()
  {
    m_pConnection = xcb_connect(NULL, NULL);
            
    const xcb_setup_t*     pSetup  = xcb_get_setup (m_pConnection);
    xcb_screen_iterator_t  iter    = xcb_setup_roots_iterator (pSetup);
    xcb_screen_t*          pScreen = iter.data;


    const uint32_t x = 0u, y = 0u, width = 800u, height = 600u, borderWidth = 10u;

    m_window = xcb_generate_id(m_pConnection);
    xcb_create_window (m_pConnection,
                       XCB_COPY_FROM_PARENT,
                       m_window,
                       pScreen->root,
                       x, y,
                       width, height,
                       borderWidth,
                       XCB_WINDOW_CLASS_INPUT_OUTPUT,
                       pScreen->root_visual,
                       0, NULL );
    xcb_map_window (m_pConnection, m_window);
    xcb_flush (m_pConnection);
  }

  void initVulkan() 
  {
    const auto& xcbSurfaceExtensions = crude_vulkan_01::XCB_Surface::requiredExtensions();
    const auto& debugUtilsExtensions = crude_vulkan_01::Debug_Utils_Messenger::requiredExtensions();

    std::vector<const char*> enabledExtensions;
    enabledExtensions.reserve(xcbSurfaceExtensions.size() + debugUtilsExtensions.size());
    enabledExtensions.insert(enabledExtensions.end(), xcbSurfaceExtensions.begin(), xcbSurfaceExtensions.end());
    enabledExtensions.insert(enabledExtensions.end(), debugUtilsExtensions.begin(), debugUtilsExtensions.end());

    // Initialize instance
    crude_vulkan_01::InstanceCreateInfo instanceInfo(debugCallback);
    instanceInfo.enabledExtensions  = enabledExtensions;
    instanceInfo.enabledLayers      = {"VK_LAYER_KHRONOS_validation"};

    m_instance = std::make_shared<crude_vulkan_01::Instance>(instanceInfo);
    
    // Initialize debugCallback
    m_debugUtilsMessenger = std::make_shared<crude_vulkan_01::Debug_Utils_Messenger>(crude_vulkan_01::DebugUtilsMessengerCreateInfo(
      m_instance,
      debugCallback));

    // Initialize surface
    m_surface = std::make_shared<crude_vulkan_01::XCB_Surface>(crude_vulkan_01::XCBSurfaceCreateInfo(
      m_instance,
      m_pConnection,
      m_window));
  }

  void cleanup() 
  {
    xcb_disconnect (m_pConnection);
  }

  static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT       messageSeverity,
                                                      VkDebugUtilsMessageTypeFlagsEXT              messageType,
                                                      const VkDebugUtilsMessengerCallbackDataEXT*  pCallbackData,
                                                      void*                                        pUserData) 
  {
    if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
      std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
    }
    return VK_FALSE;
  }

private:
  xcb_connection_t* m_pConnection;
  xcb_window_t m_window;
  std::shared_ptr<crude_vulkan_01::Instance> m_instance;
  std::shared_ptr<crude_vulkan_01::Debug_Utils_Messenger> m_debugUtilsMessenger;
  std::shared_ptr<crude_vulkan_01::Surface> m_surface;
  bool m_framebufferResized = false;
};


int main() 
{
  Test_Application testApp;

  try {
    testApp.run();
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return 0;
}
