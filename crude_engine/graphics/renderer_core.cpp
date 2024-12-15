#include <vulkan/vulkan.hpp>

module crude.gfx.renderer_core;

import crude.core.logger;
import crude.platform.sdl_window_container;
import crude.gfx.vk.physical_device;
import crude.gfx.vk.queue;
import crude.gfx.vk.instance;
import crude.gfx.vk.device;
import crude.gfx.vk.surface;
import crude.gfx.vk.swap_chain;
import crude.gfx.vk.swap_chain_image;
import crude.gfx.vk.image_view;
import crude.gfx.vk.debug_utils_messenger;
import crude.gfx.vk.command_pool;

namespace crude::gfx
{

constexpr core::array<const char* const, 7> cDeviceEnabledExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_KHR_SPIRV_1_4_EXTENSION_NAME, VK_EXT_MESH_SHADER_EXTENSION_NAME, VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME, VK_KHR_8BIT_STORAGE_EXTENSION_NAME, VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME, VK_EXT_ROBUSTNESS_2_EXTENSION_NAME };
constexpr core::array<const char*, 1> cInstanceEnabledLayers = { "VK_LAYER_KHRONOS_validation" };

Renderer_Core::Renderer_Core(core::shared_ptr<platform::SDL_Window_Container> windowContainer)
  : m_windowContainer{ windowContainer }
{
  initializeInstance();
  initializeSurface();
  initializeDevice();
  initializeSwapchain();
  initalizeCommandPool();
}

void Renderer_Core::initializeInstance()
{
  // Calculate extensions for instance
  const auto surfaceExtensions = vk::Surface::requiredExtensions();
  const auto debugUtilsExtensions = vk::Debug_Utils_Messenger::requiredExtensions();

  core::vector<const char*> enabledExtensions;
  enabledExtensions.insert(enabledExtensions.end(), surfaceExtensions.begin(), surfaceExtensions.end());
  enabledExtensions.insert(enabledExtensions.end(), debugUtilsExtensions.begin(), debugUtilsExtensions.end());

  // Set debugCallback
  auto debugCallback = [](
    VkDebugUtilsMessageSeverityFlagBitsEXT       messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT              messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) -> VKAPI_ATTR VkBool32 {
      if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
        core::logError(core::Debug::Channel::Graphics, "validation layer: %s", pCallbackData->pMessage);
      }
      else if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        core::logWarning(core::Debug::Channel::Graphics, "validation layer: %s", pCallbackData->pMessage);
      }
      return VK_FALSE;
    };

  // Set application
  const vk::Application application({
    .pApplicationName = "crude_example",
    .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
    .pEngineName = "crude_engine",
    .engineVersion = VK_MAKE_VERSION(1, 0, 0),
    .apiVersion = VK_API_VERSION_1_0 });

  // Initialize instance
  m_instance = core::allocateShared<vk::Instance>(debugCallback, application, enabledExtensions, cInstanceEnabledLayers);

  // Initialize debugCallback
  m_debugUtilsMessenger = core::allocateShared<vk::Debug_Utils_Messenger>(m_instance, debugCallback);
}

void Renderer_Core::initializeSurface()
{
  m_surface = core::allocateShared<vk::Surface>(m_instance, m_windowContainer);
}

void Renderer_Core::initializeDevice()
{
  core::shared_ptr<vk::Physical_Device> physicalDevice = pickPhysicalDevice();
  if (!physicalDevice)
  {
    core::logError(core::Debug::Channel::Graphics, "Failed to find suitable physical device!");
    return;
  }
  initializeLogicDevice(physicalDevice);
}

void Renderer_Core::initializeSwapchain()
{
  vk::Surface_Capabilities_KHR surfaceCapabilites = m_device->getPhysicalDevice()->getSurfaceCapabilitis(m_surface);
  const VkSurfaceFormatKHR surfaceFormat = m_device->getPhysicalDevice()->findSurfaceFormat(m_surface, VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR);
  const VkPresentModeKHR presentMode = m_device->getPhysicalDevice()->findSurfacePresentMode(m_surface, VK_PRESENT_MODE_MAILBOX_KHR);
  const VkExtent2D extent = surfaceCapabilites.calculateSurfaceExtentInPixels({ .width = m_windowContainer->getWidth(), .height = m_windowContainer->getHeight() });

  core::uint32 imageCount = surfaceCapabilites.getMinImageCount() + 1u;
  if (surfaceCapabilites.getMinImageCount() > 0u && imageCount > surfaceCapabilites.getMaxImageCount())
  {
    imageCount = surfaceCapabilites.getMaxImageCount();
  }

  core::vector<core::uint32> queueFamilyIndices = { m_graphicsQueue->getFamilyIndex(), m_graphicsQueue->getFamilyIndex() };

  m_swapchain = core::allocateShared<vk::Swap_Chain>(
    m_device,
    m_surface,
    surfaceFormat,
    extent,
    VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
    imageCount,
    1u,
    queueFamilyIndices,
    surfaceCapabilites.getCurrentTransform(),
    VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
    presentMode,
    true,
    0u,
    nullptr);

  m_swapchainImages = m_swapchain->getSwapchainImages();
  m_swapchainImagesViews.reserve(m_swapchainImages.size());
  for (auto swapchainImage : m_swapchainImages)
  {
    m_swapchainImagesViews.push_back(core::allocateShared<vk::Image_View>(swapchainImage, surfaceFormat.format, vk::Image_Subresource_Range(swapchainImage)));
  }
}

void Renderer_Core::initalizeCommandPool()
{
  m_graphicsCommandPool = core::allocateShared<vk::Command_Pool>(m_device, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, m_graphicsQueue->getFamilyIndex());
  m_transferCommandPool = m_graphicsCommandPool; // = core::allocateShared<Command_Pool>(m_device, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, queueIndices.transferFamily.value());
}

core::shared_ptr<vk::Physical_Device> Renderer_Core::pickPhysicalDevice()
{
  auto physicalDevices = m_instance->getPhysicalDevices();
  for (auto& physicalDevice : physicalDevices)
  {
    bool supportGraphics = false;
    bool supportPresent = false;

    // !TODO
    for (core::uint32 i = 0; i < physicalDevice->getQueueFamilyProperties().size(); ++i)
    {
      if (physicalDevice->getQueueFamilyProperties()[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) supportGraphics = true;
      if (physicalDevice->checkPresentSupport(m_surface, i)) supportPresent = true;
    }

    const bool extensionsSupported = physicalDevice->checkExtensionSupport(cDeviceEnabledExtensions);
    const bool swapChainAdequate = physicalDevice->checkSurfaceSupport(m_surface);
    const auto& supportedFeatures = physicalDevice->getFeatures();

    if (supportGraphics && supportPresent && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy)
    {
      core::logInfo(core::Debug::Channel::Graphics, "Selected physical device %s %i", physicalDevice->getProperties().getDeviceName(), physicalDevice->getProperties().getDeviceType());
      return physicalDevice;
    }
  }
  return nullptr;
}

void Renderer_Core::initializeLogicDevice(core::shared_ptr<const vk::Physical_Device> physicalDevice)
{
  VkPhysicalDeviceFeatures deviceFeatures{};
  deviceFeatures.samplerAnisotropy = VK_TRUE;
  deviceFeatures.sampleRateShading = VK_TRUE;
  deviceFeatures.independentBlend  = VK_TRUE;
  deviceFeatures.imageCubeArray    = VK_TRUE;

  core::array<vk::Device_Queue_Descriptor, 2> queueInfos =
  {
    vk::Device_Queue_Descriptor(physicalDevice, VK_QUEUE_GRAPHICS_BIT),
    vk::Device_Queue_Descriptor(physicalDevice, m_surface),
  };

  m_device = core::allocateShared<vk::Device>(physicalDevice, queueInfos, deviceFeatures, cDeviceEnabledExtensions, cInstanceEnabledLayers);

  for (auto it = m_device->getQueueDescriptors().begin(); it != m_device->getQueueDescriptors().end(); ++it)
  {
    if (it->getDenotation() == vk::DEVICE_QUEUE_DENOTATION_GRAPHICS_BITS)
      m_graphicsQueue = m_device->getQueueByFamily(it->queueFamilyIndex, 0u).value_or(nullptr);
    if (it->getDenotation() == vk::DEVICE_QUEUE_DENOTATION_PRESENT_BITS)
      m_presentQueue = m_device->getQueueByFamily(it->queueFamilyIndex, 0u).value_or(nullptr);
  }
  if (!m_presentQueue) m_presentQueue = m_graphicsQueue;
  if (!m_transferQueue) m_transferQueue = m_graphicsQueue;
}
core::shared_ptr<vk::Queue> Renderer_Core::getGraphicsQueue() { return m_graphicsQueue; }
core::shared_ptr<vk::Queue> Renderer_Core::getPresentQueue() { return m_presentQueue; }
core::shared_ptr<vk::Queue> Renderer_Core::getTransferQueue() { return m_transferQueue; }
core::shared_ptr<vk::Command_Pool> Renderer_Core::getGraphicsCommandPool() { return m_graphicsCommandPool; }
core::shared_ptr<vk::Command_Pool> Renderer_Core::getTransferCommandPool() { return m_transferCommandPool; }
core::shared_ptr<vk::Instance> Renderer_Core::getInstance() { return m_instance; }
core::shared_ptr<vk::Device> Renderer_Core::getDevice() { return m_device; }
core::shared_ptr<vk::Surface> Renderer_Core::getSurface() { return m_surface; }
core::shared_ptr<vk::Swap_Chain> Renderer_Core::getSwapchain() { return m_swapchain; }
const core::vector<core::shared_ptr<vk::Swap_Chain_Image>>& Renderer_Core::getSwapchainImages() { return m_swapchainImages; }
const core::vector<core::shared_ptr<vk::Image_View>>& Renderer_Core::getSwapchainImagesViews() { return m_swapchainImagesViews; }
core::shared_ptr<vk::Debug_Utils_Messenger> Renderer_Core::getDebugUtilsMessenger() { return m_debugUtilsMessenger; }
core::shared_ptr<platform::SDL_Window_Container> Renderer_Core::getWindowContainer() { return m_windowContainer; }

}