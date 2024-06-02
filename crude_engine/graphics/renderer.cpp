#include <vulkan/vulkan.hpp>

module crude.graphics.renderer;

import crude.core.logger;

namespace crude::graphics
{

Renderer::Renderer()
{
  initializeInstance();
}

void Renderer::initializeInstance()
{
  // Calculate extensions for instance
  const auto surfaceExtensions = Surface::requiredExtensions();
  const auto debugUtilsExtensions = Debug_Utils_Messenger::requiredExtensions();
  
  core::vector<const char*> enabledExtensions(surfaceExtensions.size() + debugUtilsExtensions.size());
  for (core::size_t i = 0u; i < surfaceExtensions.size(); ++i)
  {
    enabledExtensions[i] = surfaceExtensions[i];
  }
  for (core::size_t i = 0u; i < debugUtilsExtensions.size(); ++i)
  {
    enabledExtensions[i + surfaceExtensions.size()] = debugUtilsExtensions[i];
  }
  
  
  auto debugCallback = [](VkDebugUtilsMessageSeverityFlagBitsEXT       messageSeverity,
                          VkDebugUtilsMessageTypeFlagsEXT              messageType,
                          const VkDebugUtilsMessengerCallbackDataEXT*  pCallbackData,
                          void*                                        pUserData) -> VKAPI_ATTR VkBool32 {
      core::log(core::Debug::Channel::Graphics, "validation layer: %s", pCallbackData->pMessage);
      return VK_FALSE;
   };

  // Initialize instance
  const Application application("crude_example", VK_MAKE_VERSION(1, 0, 0), "crude_engine", VK_MAKE_VERSION(1, 0, 0));

  core::array<const char*, 1u> enabledLayers = { "VK_LAYER_KHRONOS_validation" };
  m_instance = core::makeShared<Instance>(debugCallback, application, enabledExtensions, enabledLayers);

  // Initialize debugCallback
  m_debugUtilsMessenger = core::makeShared<Debug_Utils_Messenger>(m_instance, debugCallback);
}

}