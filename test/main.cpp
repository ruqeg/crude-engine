#define VK_EXT_debug_utils
#include "../include/crude_vulkan_01/instance.hpp"
#include "../include/crude_vulkan_01/debug_utils_messenger.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>

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
    glfwInit();
    
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_window = glfwCreateWindow(800, 600, "Test_Application", nullptr, nullptr);

    if (m_window == nullptr)
    {
      throw std::runtime_error("Failed to glfwCreateWindow");
    }

    glfwSetWindowAttrib(m_window, GLFW_RESIZABLE, GLFW_TRUE);

    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
  }

  static void framebufferResizeCallback(GLFWwindow* window, int width, int height)
  {
    auto app = reinterpret_cast<Test_Application*>(glfwGetWindowUserPointer(window));
    app->m_framebufferResized = true; 
  }

  void initVulkan() 
  {
    // Initialize instance
    crude_vulkan_01::InstanceCreateInfo instanceInfo(debugCallback);
    instanceInfo.enabledExtensions  = getRequiredExtensions();
    instanceInfo.enabledLayers      = {"VK_LAYER_KHRONOS_validation"};

    m_instance = std::make_shared<crude_vulkan_01::Instance>(instanceInfo);
    
    // Initialize debugCallback
    m_debugUtilsMessenger = std::make_shared<crude_vulkan_01::Debug_Utils_Messenger>(crude_vulkan_01::DebugUtilsMessengerCreateInfo(
      m_instance,
      debugCallback));
  }

  std::vector<const char*> getRequiredExtensions()
  {
    crude_vulkan_01::uint32 glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    return extensions;
  }

  void cleanup() 
  {
    glfwDestroyWindow(m_window);
    glfwTerminate(); 
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
  GLFWwindow* m_window;
  std::shared_ptr<crude_vulkan_01::Instance> m_instance;
  std::shared_ptr<crude_vulkan_01::Debug_Utils_Messenger> m_debugUtilsMessenger;
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
