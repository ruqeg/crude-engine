#include <vulkan/vulkan.hpp>

module crude.gfx.renderer_core_system;

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

Renderer_Core_System_Ctx::Renderer_Core_System_Ctx(core::shared_ptr<platform::SDL_Window_Container> windowContainer)
  : windowContainer{ windowContainer }
{
  initializeInstance();
  initializeSurface();
  initializeDevice();
  initializeSwapchain();
  initalizeCommandPool();
}

void Renderer_Core_System_Ctx::initializeInstance()
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
  instance = core::allocateShared<vk::Instance>(debugCallback, application, enabledExtensions, cInstanceEnabledLayers);

  // Initialize debugCallback
  debugUtilsMessenger = core::allocateShared<vk::Debug_Utils_Messenger>(instance, debugCallback);
}

void Renderer_Core_System_Ctx::initializeSurface()
{
  surface = core::allocateShared<vk::Surface>(instance, windowContainer);
}

void Renderer_Core_System_Ctx::initializeDevice()
{
  core::shared_ptr<vk::Physical_Device> physicalDevice = pickPhysicalDevice();
  if (!physicalDevice)
  {
    core::logError(core::Debug::Channel::Graphics, "Failed to find suitable physical device!");
    return;
  }
  initializeLogicDevice(physicalDevice);
}

void Renderer_Core_System_Ctx::initializeSwapchain()
{
  vk::Surface_Capabilities_KHR surfaceCapabilites = device->getPhysicalDevice()->getSurfaceCapabilitis(surface);
  const VkSurfaceFormatKHR surfaceFormat = device->getPhysicalDevice()->findSurfaceFormat(surface, VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR);
  const VkPresentModeKHR presentMode = device->getPhysicalDevice()->findSurfacePresentMode(surface, VK_PRESENT_MODE_MAILBOX_KHR);
  const VkExtent2D extent = surfaceCapabilites.calculateSurfaceExtentInPixels({ .width = windowContainer->getWidth(), .height = windowContainer->getHeight() });

  core::uint32 imageCount = surfaceCapabilites.getMinImageCount() + 1u;
  if (surfaceCapabilites.getMinImageCount() > 0u && imageCount > surfaceCapabilites.getMaxImageCount())
  {
    imageCount = surfaceCapabilites.getMaxImageCount();
  }

  core::vector<core::uint32> queueFamilyIndices = { graphicsQueue->getFamilyIndex(), graphicsQueue->getFamilyIndex() };

  swapchain = core::allocateShared<vk::Swap_Chain>(
    device,
    surface,
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

  swapchainImages = swapchain->getSwapchainImages();
  swapchainImagesViews.resize(swapchainImages.size());
  for (core::uint32 i = 0; i < swapchainImages.size(); ++i)
  {
    swapchainImagesViews[i] = core::allocateShared<vk::Image_View>(swapchainImages[i], surfaceFormat.format, vk::Image_Subresource_Range(swapchainImages[i]));
  }
}

void Renderer_Core_System_Ctx::initalizeCommandPool()
{
  graphicsCommandPool = core::allocateShared<vk::Command_Pool>(device, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, graphicsQueue->getFamilyIndex());
  transferCommandPool = graphicsCommandPool; // = core::allocateShared<Command_Pool>(m_device, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, queueIndices.transferFamily.value());
}

core::shared_ptr<vk::Physical_Device> Renderer_Core_System_Ctx::pickPhysicalDevice()
{
  auto physicalDevices = instance->getPhysicalDevices();
  for (auto& physicalDevice : physicalDevices)
  {
    bool supportGraphics = false;
    bool supportPresent = false;

    // !TODO
    for (core::uint32 i = 0; i < physicalDevice->getQueueFamilyProperties().size(); ++i)
    {
      if (physicalDevice->getQueueFamilyProperties()[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) supportGraphics = true;
      if (physicalDevice->checkPresentSupport(surface, i)) supportPresent = true;
    }

    const bool extensionsSupported = physicalDevice->checkExtensionSupport(cDeviceEnabledExtensions);
    const bool swapChainAdequate = physicalDevice->checkSurfaceSupport(surface);
    const auto& supportedFeatures = physicalDevice->getFeatures();

    if (supportGraphics && supportPresent && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy)
    {
      core::logInfo(core::Debug::Channel::Graphics, "Selected physical device %s %i", physicalDevice->getProperties().getDeviceName(), physicalDevice->getProperties().getDeviceType());
      return physicalDevice;
    }
  }
  return nullptr;
}

void Renderer_Core_System_Ctx::initializeLogicDevice(core::shared_ptr<const vk::Physical_Device> physicalDevice)
{
  VkPhysicalDeviceFeatures deviceFeatures{};
  deviceFeatures.samplerAnisotropy = VK_TRUE;
  deviceFeatures.sampleRateShading = VK_TRUE;
  deviceFeatures.independentBlend  = VK_TRUE;
  deviceFeatures.imageCubeArray    = VK_TRUE;

  core::array<vk::Device_Queue_Descriptor, 2> queueInfos =
  {
    vk::Device_Queue_Descriptor(physicalDevice, VK_QUEUE_GRAPHICS_BIT),
    vk::Device_Queue_Descriptor(physicalDevice, surface),
  };

  device = core::allocateShared<vk::Device>(physicalDevice, queueInfos, deviceFeatures, cDeviceEnabledExtensions, cInstanceEnabledLayers);

  for (auto it = device->getQueueDescriptors().begin(); it != device->getQueueDescriptors().end(); ++it)
  {
    if (it->getDenotation() == vk::DEVICE_QUEUE_DENOTATION_GRAPHICS_BITS)
      graphicsQueue = device->getQueueByFamily(it->queueFamilyIndex, 0u).value_or(nullptr);
    if (it->getDenotation() == vk::DEVICE_QUEUE_DENOTATION_PRESENT_BITS)
      presentQueue = device->getQueueByFamily(it->queueFamilyIndex, 0u).value_or(nullptr);
  }
  if (!presentQueue) presentQueue = graphicsQueue;
  if (!transferQueue) transferQueue = graphicsQueue;
}

}