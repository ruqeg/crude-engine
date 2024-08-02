#include <vulkan/vulkan.hpp>

module crude.graphics.renderer_base;

import crude.core.logger;
import crude.platform.sdl_window_container;
import crude.graphics.physical_device;
import crude.graphics.queue;
import crude.graphics.instance;
import crude.graphics.device;
import crude.graphics.surface;
import crude.graphics.swap_chain;
import crude.graphics.swap_chain_image;
import crude.graphics.image_view;
import crude.graphics.debug_utils_messenger;

namespace crude::graphics
{

constexpr core::array<const char* const, 6> cDeviceEnabledExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_KHR_SPIRV_1_4_EXTENSION_NAME, VK_EXT_MESH_SHADER_EXTENSION_NAME, VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME, VK_KHR_8BIT_STORAGE_EXTENSION_NAME, VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME };
constexpr core::array<const char*, 1> cInstanceEnabledLayers = { "VK_LAYER_KHRONOS_validation" };

Renderer_Base::Renderer_Base(core::shared_ptr<platform::SDL_Window_Container> windowContainer)
  : m_windowContainer(windowContainer)
{
  initializeInstance();
  initializeSurface();
  initializeDevice();
  initializeSwapchain();
}

Renderer_Base::~Renderer_Base()
{
  m_device->waitIdle();
}

void Renderer_Base::initializeInstance()
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
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) -> VKAPI_ATTR VkBool32 {
      core::log(core::Debug::Channel::Graphics, "validation layer: %s", pCallbackData->pMessage);
      return VK_FALSE;
    };

  // Initialize instance
  const Application application({
    .pApplicationName = "crude_example",
    .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
    .pEngineName = "crude_engine",
    .engineVersion = VK_MAKE_VERSION(1, 0, 0),
    .apiVersion = VK_API_VERSION_1_0 });

  m_instance = core::allocateShared<Instance>(debugCallback, application, enabledExtensions, cInstanceEnabledLayers);

  // Initialize debugCallback
  m_debugUtilsMessenger = core::allocateShared<Debug_Utils_Messenger>(m_instance, debugCallback);
}

void Renderer_Base::initializeSurface()
{
  m_surface = core::allocateShared<Surface>(m_instance, m_windowContainer);
}

void Renderer_Base::initializeDevice()
{
  core::shared_ptr<Physical_Device> physicalDevice = pickPhysicalDevice();
  if (!physicalDevice)
  {
    core::logError(core::Debug::Channel::Graphics, "Failed to find suitable physical device!");
    return;
  }
  initializeLogicDevice(physicalDevice);
}

void Renderer_Base::initializeSwapchain()
{
  Surface_Capabilities_KHR surfaceCapabilites = m_device->getPhysicalDevice()->getSurfaceCapabilitis(m_surface);
  const VkSurfaceFormatKHR surfaceFormat = m_device->getPhysicalDevice()->findSurfaceFormat(m_surface, VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR);
  const VkPresentModeKHR presentMode = m_device->getPhysicalDevice()->findSurfacePresentMode(m_surface, VK_PRESENT_MODE_MAILBOX_KHR);
  const VkExtent2D extent = surfaceCapabilites.calculateSurfaceExtentInPixels({ .width = m_windowContainer->getWidth(), .height = m_windowContainer->getHeight() });

  core::uint32 imageCount = surfaceCapabilites.getMinImageCount() + 1u;
  if (surfaceCapabilites.getMinImageCount() > 0u && imageCount > surfaceCapabilites.getMaxImageCount())
  {
    imageCount = surfaceCapabilites.getMaxImageCount();
  }

  core::vector<core::uint32> queueFamilyIndices = { m_graphicsQueue->getFamilyIndex(), m_presentQueue->getFamilyIndex() };

  m_swapchain = core::allocateShared<Swap_Chain>(
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
  m_swapchainImagesViews.resize(m_swapchainImages.size());
  for (core::uint32 i = 0; i < m_swapchainImages.size(); ++i)
  {
    m_swapchainImagesViews[i] = core::allocateShared<Image_View>(m_swapchainImages[i], surfaceFormat.format, Image_Subresource_Range(m_swapchainImages[i]));
  }
}

core::shared_ptr<Physical_Device> Renderer_Base::pickPhysicalDevice()
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

void Renderer_Base::initializeLogicDevice(core::shared_ptr<const Physical_Device> physicalDevice)
{
  VkPhysicalDeviceFeatures deviceFeatures{};
  deviceFeatures.samplerAnisotropy = VK_TRUE;
  deviceFeatures.sampleRateShading = VK_TRUE;

  core::array<Device_Queue_Descriptor, 2> queueInfos =
  {
    Device_Queue_Descriptor(physicalDevice, VK_QUEUE_GRAPHICS_BIT),
    Device_Queue_Descriptor(physicalDevice, m_surface),
  };

  m_device = core::allocateShared<Device>(physicalDevice, queueInfos, deviceFeatures, cDeviceEnabledExtensions, cInstanceEnabledLayers);

  for (auto it = m_device->getQueueDescriptors().begin(); it != m_device->getQueueDescriptors().end(); ++it)
  {
    if (it->getDenotation() == DEVICE_QUEUE_DENOTATION_GRAPHICS_BITS)
      m_graphicsQueue = m_device->getQueueByFamily(it->queueFamilyIndex, 0u).valueOr(nullptr);
    if (it->getDenotation() == DEVICE_QUEUE_DENOTATION_PRESENT_BITS)
      m_presentQueue = m_device->getQueueByFamily(it->queueFamilyIndex, 0u).valueOr(nullptr);
  }
  if (!m_presentQueue) m_presentQueue = m_graphicsQueue;
  if (!m_transferQueue) m_transferQueue = m_graphicsQueue;
}

core::shared_ptr<Queue> Renderer_Base::getGraphicsQueue() { return m_graphicsQueue; }
core::shared_ptr<Queue> Renderer_Base::getPresentQueue() { return m_presentQueue; }
core::shared_ptr<Queue> Renderer_Base::getTransferQueue() { return m_transferQueue; }
core::shared_ptr<Instance> Renderer_Base::getInstance() { return m_instance; }
core::shared_ptr<Device> Renderer_Base::getDevice() { return m_device; }
core::shared_ptr<Surface> Renderer_Base::getSurface() { return m_surface; }
core::shared_ptr<Swap_Chain> Renderer_Base::getSwapchain() { return m_swapchain; }
const core::vector<core::shared_ptr<Swap_Chain_Image>>& Renderer_Base::getSwapchainImages() { return m_swapchainImages; }
const core::vector<core::shared_ptr<Image_View>>& Renderer_Base::getSwapchainImagesViews() { return m_swapchainImagesViews; }
core::shared_ptr<Debug_Utils_Messenger> Renderer_Base::getDebugUtilsMessenger() { return m_debugUtilsMessenger; }
core::shared_ptr<platform::SDL_Window_Container> Renderer_Base::getWindowContainer() { return m_windowContainer; }

}