#include <vulkan/vulkan.hpp>

module crude.graphics.renderer;

import crude.core.logger;

namespace crude::graphics
{

constexpr core::array<const char*, 1> deviceEnabledExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
constexpr core::array<const char*, 1> instanceEnabledLayers = { "VK_LAYER_KHRONOS_validation" };

Renderer::Renderer(core::Shared_Ptr<system::SDL_Window_Container> windowContainer)
  :
  m_windowContainer(windowContainer)
{
  initializeInstance();
  initializeSurface();
  initializeDevice();
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

  m_instance = core::makeShared<Instance>(debugCallback, application, enabledExtensions, instanceEnabledLayers);

  // Initialize debugCallback
  m_debugUtilsMessenger = core::makeShared<Debug_Utils_Messenger>(m_instance, debugCallback);
}

void Renderer::initializeSurface()
{
  m_surface = core::makeShared<Surface>(m_instance, m_windowContainer);
}

void Renderer::initializeDevice()
{
  core::Shared_Ptr<Physical_Device> physicalDevice = pickPhysicalDevice();
  if (!physicalDevice)
  {
    core::logError(core::Debug::Channel::Graphics, "Failed to find suitable physical device!");
    return;
  }
  initializeLogicDevice(physicalDevice);
}

core::Shared_Ptr<Physical_Device> Renderer::pickPhysicalDevice()
{
  auto physicalDevices = m_instance->getPhysicalDevices();
  for (auto& physicalDevice : physicalDevices)
  {
    const Queue_Family_Indices queueIndices = findDeviceQueueFamilies(*physicalDevice);
    const bool extensionsSupported = checkDeviceExtensionSupport(*physicalDevice);
    const bool swapChainAdequate = checkSurfaceSupport(*physicalDevice, m_surface);
    const auto& supportedFeatures = physicalDevice->getFeatures();

    if (queueIndices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy)
    {
      core::logInfo(core::Debug::Channel::Graphics, "Selected physical device %s %i", physicalDevice->getProperties().deviceName, physicalDevice->getProperties().deviceType);
      return physicalDevice;
    }
  }
  return nullptr;
}

Renderer::Queue_Family_Indices Renderer::findDeviceQueueFamilies(core::Shared_Ptr<Physical_Device> physicalDevice)
{
  Queue_Family_Indices indices;
  
  const auto& queueProperties = physicalDevice->getQueueFamilyProperties();
  for (core::uint32 i = 0; i < queueProperties.size(); ++i)
  {
    if (queueProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
      indices.graphicsFamily = i;
  
    if (physicalDevice->getSupportSurface(m_surface, i))
      indices.presentFamily = i;
  }

  return indices;
}

bool Renderer::checkDeviceExtensionSupport(core::Shared_Ptr<Physical_Device> physicalDevice)
{
  static const core::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
  
  const auto& availableExtensions = physicalDevice->getExtensionProperties();
  core::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
  
  for (const auto& extension : availableExtensions)
  {
    requiredExtensions.erase(extension.extensionName);
  }
  
  return requiredExtensions.empty();
}

bool Renderer::checkSurfaceSupport(core::Shared_Ptr<Physical_Device> physicalDevice, core::Shared_Ptr<Surface> surface)
{
  return !physicalDevice->getSurfaceFormats(surface).empty() && !physicalDevice->getSurfacePresentModes(surface).empty();
}

void Renderer::initializeLogicDevice(core::Shared_Ptr<Physical_Device> physicalDevice)
{
  const Queue_Family_Indices queueIndices = findDeviceQueueFamilies(physicalDevice);

  VkPhysicalDeviceFeatures deviceFeatures{};
  deviceFeatures.samplerAnisotropy = VK_TRUE;

  float queuePriorities[] = { 1.f };

  core::array<Device_Queue_Create_Info, 2> deviceQueueCreateInfos =
  {
    Device_Queue_Create_Info(queueIndices.graphicsFamily.value(), queuePriorities),
    Device_Queue_Create_Info(queueIndices.presentFamily.value(), queuePriorities)
  };

  m_device = core::makeShared<Device>(physicalDevice, deviceQueueCreateInfos, deviceFeatures, deviceEnabledExtensions, instanceEnabledLayers);

  m_graphicsQueue = m_device->getQueue(queueIndices.graphicsFamily.value(), 0u);
  m_presentQueue = m_device->getQueue(queueIndices.presentFamily.value(), 0u);
}

}