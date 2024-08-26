#include <flecs.h>
#include <vulkan/vulkan.hpp>

module crude.graphics.renderer_core_component;

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
import crude.graphics.command_pool;

namespace crude::graphics
{

constexpr core::array<const char* const, 6> cDeviceEnabledExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_KHR_SPIRV_1_4_EXTENSION_NAME, VK_EXT_MESH_SHADER_EXTENSION_NAME, VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME, VK_KHR_8BIT_STORAGE_EXTENSION_NAME, VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME };
constexpr core::array<const char*, 1> cInstanceEnabledLayers = { "VK_LAYER_KHRONOS_validation" };

void rendererCoreComponentInitialize(flecs::iter& it)
{
  Renderer_Core_Component rendererCoreComponent;

  rendererCoreComponent.windowContainer = *it.world().get_mut<core::shared_ptr<platform::SDL_Window_Container>>();

  initializeInstance(&rendererCoreComponent);
  initializeSurface(&rendererCoreComponent);
  initializeDevice(&rendererCoreComponent);
  initializeSwapchain(&rendererCoreComponent);
  initalizeCommandPool(&rendererCoreComponent);

  it.world().set<Renderer_Core_Component>(std::move(rendererCoreComponent));
}

void rendererCoreComponentDeinitialize(flecs::iter& it)
{
  it.world().get_mut<Renderer_Core_Component>()->device->waitIdle();
}
 
void initializeInstance(Renderer_Core_Component* rendererCoreComponent)
{
  // Calculate extensions for instance
  const auto surfaceExtensions = Surface::requiredExtensions();
  const auto debugUtilsExtensions = Debug_Utils_Messenger::requiredExtensions();

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
  const Application application({
    .pApplicationName = "crude_example",
    .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
    .pEngineName = "crude_engine",
    .engineVersion = VK_MAKE_VERSION(1, 0, 0),
    .apiVersion = VK_API_VERSION_1_0 });

  // Initialize instance
  rendererCoreComponent->instance = core::allocateShared<Instance>(debugCallback, application, enabledExtensions, cInstanceEnabledLayers);

  // Initialize debugCallback
  rendererCoreComponent->debugUtilsMessenger = core::allocateShared<Debug_Utils_Messenger>(rendererCoreComponent->instance, debugCallback);
}

void initializeSurface(Renderer_Core_Component* rendererCoreComponent)
{
  rendererCoreComponent->surface = core::allocateShared<Surface>(rendererCoreComponent->instance, rendererCoreComponent->windowContainer);
}

void initializeDevice(Renderer_Core_Component* rendererCoreComponent)
{
  core::shared_ptr<Physical_Device> physicalDevice = pickPhysicalDevice(rendererCoreComponent);
  if (!physicalDevice)
  {
    core::logError(core::Debug::Channel::Graphics, "Failed to find suitable physical device!");
    return;
  }
  initializeLogicDevice(rendererCoreComponent, physicalDevice);
}

void initializeSwapchain(Renderer_Core_Component* rendererCoreComponent)
{
  Surface_Capabilities_KHR surfaceCapabilites = rendererCoreComponent->device->getPhysicalDevice()->getSurfaceCapabilitis(rendererCoreComponent->surface);
  const VkSurfaceFormatKHR surfaceFormat = rendererCoreComponent->device->getPhysicalDevice()->findSurfaceFormat(rendererCoreComponent->surface, VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR);
  const VkPresentModeKHR presentMode = rendererCoreComponent->device->getPhysicalDevice()->findSurfacePresentMode(rendererCoreComponent->surface, VK_PRESENT_MODE_MAILBOX_KHR);
  const VkExtent2D extent = surfaceCapabilites.calculateSurfaceExtentInPixels({ .width = rendererCoreComponent->windowContainer->getWidth(), .height = rendererCoreComponent->windowContainer->getHeight() });

  core::uint32 imageCount = surfaceCapabilites.getMinImageCount() + 1u;
  if (surfaceCapabilites.getMinImageCount() > 0u && imageCount > surfaceCapabilites.getMaxImageCount())
  {
    imageCount = surfaceCapabilites.getMaxImageCount();
  }

  core::vector<core::uint32> queueFamilyIndices = { rendererCoreComponent->graphicsQueue->getFamilyIndex(), rendererCoreComponent->graphicsQueue->getFamilyIndex() };

  rendererCoreComponent->swapchain = core::allocateShared<Swap_Chain>(
    rendererCoreComponent->device,
    rendererCoreComponent->surface,
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

  rendererCoreComponent->swapchainImages = rendererCoreComponent->swapchain->getSwapchainImages();
  rendererCoreComponent->swapchainImagesViews.resize(rendererCoreComponent->swapchainImages.size());
  for (core::uint32 i = 0; i < rendererCoreComponent->swapchainImages.size(); ++i)
  {
    rendererCoreComponent->swapchainImagesViews[i] = core::allocateShared<Image_View>(rendererCoreComponent->swapchainImages[i], surfaceFormat.format, Image_Subresource_Range(rendererCoreComponent->swapchainImages[i]));
  }
}

void initalizeCommandPool(Renderer_Core_Component* rendererCoreComponent)
{
  rendererCoreComponent->graphicsCommandPool = core::allocateShared<Command_Pool>(rendererCoreComponent->device, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, rendererCoreComponent->graphicsQueue->getFamilyIndex());
  rendererCoreComponent->transferCommandPool = rendererCoreComponent->graphicsCommandPool; // = core::allocateShared<Command_Pool>(m_device, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, queueIndices.transferFamily.value());
}

core::shared_ptr<Physical_Device> pickPhysicalDevice(Renderer_Core_Component* rendererCoreComponent)
{
  auto physicalDevices = rendererCoreComponent->instance->getPhysicalDevices();
  for (auto& physicalDevice : physicalDevices)
  {
    bool supportGraphics = false;
    bool supportPresent = false;

    // !TODO
    for (core::uint32 i = 0; i < physicalDevice->getQueueFamilyProperties().size(); ++i)
    {
      if (physicalDevice->getQueueFamilyProperties()[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) supportGraphics = true;
      if (physicalDevice->checkPresentSupport(rendererCoreComponent->surface, i)) supportPresent = true;
    }

    const bool extensionsSupported = physicalDevice->checkExtensionSupport(cDeviceEnabledExtensions);
    const bool swapChainAdequate = physicalDevice->checkSurfaceSupport(rendererCoreComponent->surface);
    const auto& supportedFeatures = physicalDevice->getFeatures();

    if (supportGraphics && supportPresent && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy)
    {
      core::logInfo(core::Debug::Channel::Graphics, "Selected physical device %s %i", physicalDevice->getProperties().getDeviceName(), physicalDevice->getProperties().getDeviceType());
      return physicalDevice;
    }
  }
  return nullptr;
}

void initializeLogicDevice(Renderer_Core_Component* rendererCoreComponent, core::shared_ptr<const Physical_Device> physicalDevice)
{
  VkPhysicalDeviceFeatures deviceFeatures{};
  deviceFeatures.samplerAnisotropy = VK_TRUE;
  deviceFeatures.sampleRateShading = VK_TRUE;

  core::array<Device_Queue_Descriptor, 2> queueInfos =
  {
    Device_Queue_Descriptor(physicalDevice, VK_QUEUE_GRAPHICS_BIT),
    Device_Queue_Descriptor(physicalDevice, rendererCoreComponent->surface),
  };

  rendererCoreComponent->device = core::allocateShared<Device>(physicalDevice, queueInfos, deviceFeatures, cDeviceEnabledExtensions, cInstanceEnabledLayers);

  for (auto it = rendererCoreComponent->device->getQueueDescriptors().begin(); it != rendererCoreComponent->device->getQueueDescriptors().end(); ++it)
  {
    if (it->getDenotation() == DEVICE_QUEUE_DENOTATION_GRAPHICS_BITS)
      rendererCoreComponent->graphicsQueue = rendererCoreComponent->device->getQueueByFamily(it->queueFamilyIndex, 0u).value_or(nullptr);
    if (it->getDenotation() == DEVICE_QUEUE_DENOTATION_PRESENT_BITS)
      rendererCoreComponent->presentQueue = rendererCoreComponent->device->getQueueByFamily(it->queueFamilyIndex, 0u).value_or(nullptr);
  }
  if (!rendererCoreComponent->presentQueue) rendererCoreComponent->presentQueue = rendererCoreComponent->graphicsQueue;
  if (!rendererCoreComponent->transferQueue) rendererCoreComponent->transferQueue = rendererCoreComponent->graphicsQueue;
}

}