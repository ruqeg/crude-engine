#include <flecs.h>
#include <vulkan/vulkan.hpp>

module crude.graphics.renderer_core_system;

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

void rendererCoreSystemInitialize(flecs::iter& it)
{
  Renderer_Core_System_Ctx* ctx = it.ctx<Renderer_Core_System_Ctx>();

  initializeInstance(ctx);
  initializeSurface(ctx);
  initializeDevice(ctx);
  initializeSwapchain(ctx);
  initalizeCommandPool(ctx);
}

void rendererCoreSystemDeinitialize(flecs::iter& it)
{
  Renderer_Core_System_Ctx* ctx = it.ctx<Renderer_Core_System_Ctx>();
  ctx->device->waitIdle();
}
 
void initializeInstance(Renderer_Core_System_Ctx* ctx)
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
  ctx->instance = core::allocateShared<Instance>(debugCallback, application, enabledExtensions, cInstanceEnabledLayers);

  // Initialize debugCallback
  ctx->debugUtilsMessenger = core::allocateShared<Debug_Utils_Messenger>(ctx->instance, debugCallback);
}

void initializeSurface(Renderer_Core_System_Ctx* ctx)
{
  ctx->surface = core::allocateShared<Surface>(ctx->instance, ctx->windowContainer);
}

void initializeDevice(Renderer_Core_System_Ctx* ctx)
{
  core::shared_ptr<Physical_Device> physicalDevice = pickPhysicalDevice(ctx);
  if (!physicalDevice)
  {
    core::logError(core::Debug::Channel::Graphics, "Failed to find suitable physical device!");
    return;
  }
  initializeLogicDevice(ctx, physicalDevice);
}

void initializeSwapchain(Renderer_Core_System_Ctx* ctx)
{
  Surface_Capabilities_KHR surfaceCapabilites = ctx->device->getPhysicalDevice()->getSurfaceCapabilitis(ctx->surface);
  const VkSurfaceFormatKHR surfaceFormat = ctx->device->getPhysicalDevice()->findSurfaceFormat(ctx->surface, VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR);
  const VkPresentModeKHR presentMode = ctx->device->getPhysicalDevice()->findSurfacePresentMode(ctx->surface, VK_PRESENT_MODE_MAILBOX_KHR);
  const VkExtent2D extent = surfaceCapabilites.calculateSurfaceExtentInPixels({ .width = ctx->windowContainer->getWidth(), .height = ctx->windowContainer->getHeight() });

  core::uint32 imageCount = surfaceCapabilites.getMinImageCount() + 1u;
  if (surfaceCapabilites.getMinImageCount() > 0u && imageCount > surfaceCapabilites.getMaxImageCount())
  {
    imageCount = surfaceCapabilites.getMaxImageCount();
  }

  core::vector<core::uint32> queueFamilyIndices = { ctx->graphicsQueue->getFamilyIndex(), ctx->graphicsQueue->getFamilyIndex() };

  ctx->swapchain = core::allocateShared<Swap_Chain>(
    ctx->device,
    ctx->surface,
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

  ctx->swapchainImages = ctx->swapchain->getSwapchainImages();
  ctx->swapchainImagesViews.resize(ctx->swapchainImages.size());
  for (core::uint32 i = 0; i < ctx->swapchainImages.size(); ++i)
  {
    ctx->swapchainImagesViews[i] = core::allocateShared<Image_View>(ctx->swapchainImages[i], surfaceFormat.format, Image_Subresource_Range(ctx->swapchainImages[i]));
  }
}

void initalizeCommandPool(Renderer_Core_System_Ctx* ctx)
{
  ctx->graphicsCommandPool = core::allocateShared<Command_Pool>(ctx->device, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, ctx->graphicsQueue->getFamilyIndex());
  ctx->transferCommandPool = ctx->graphicsCommandPool; // = core::allocateShared<Command_Pool>(m_device, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, queueIndices.transferFamily.value());
}

core::shared_ptr<Physical_Device> pickPhysicalDevice(Renderer_Core_System_Ctx* ctx)
{
  auto physicalDevices = ctx->instance->getPhysicalDevices();
  for (auto& physicalDevice : physicalDevices)
  {
    bool supportGraphics = false;
    bool supportPresent = false;

    // !TODO
    for (core::uint32 i = 0; i < physicalDevice->getQueueFamilyProperties().size(); ++i)
    {
      if (physicalDevice->getQueueFamilyProperties()[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) supportGraphics = true;
      if (physicalDevice->checkPresentSupport(ctx->surface, i)) supportPresent = true;
    }

    const bool extensionsSupported = physicalDevice->checkExtensionSupport(cDeviceEnabledExtensions);
    const bool swapChainAdequate = physicalDevice->checkSurfaceSupport(ctx->surface);
    const auto& supportedFeatures = physicalDevice->getFeatures();

    if (supportGraphics && supportPresent && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy)
    {
      core::logInfo(core::Debug::Channel::Graphics, "Selected physical device %s %i", physicalDevice->getProperties().getDeviceName(), physicalDevice->getProperties().getDeviceType());
      return physicalDevice;
    }
  }
  return nullptr;
}

void initializeLogicDevice(Renderer_Core_System_Ctx* ctx, core::shared_ptr<const Physical_Device> physicalDevice)
{
  VkPhysicalDeviceFeatures deviceFeatures{};
  deviceFeatures.samplerAnisotropy = VK_TRUE;
  deviceFeatures.sampleRateShading = VK_TRUE;

  core::array<Device_Queue_Descriptor, 2> queueInfos =
  {
    Device_Queue_Descriptor(physicalDevice, VK_QUEUE_GRAPHICS_BIT),
    Device_Queue_Descriptor(physicalDevice, ctx->surface),
  };

  ctx->device = core::allocateShared<Device>(physicalDevice, queueInfos, deviceFeatures, cDeviceEnabledExtensions, cInstanceEnabledLayers);

  for (auto it = ctx->device->getQueueDescriptors().begin(); it != ctx->device->getQueueDescriptors().end(); ++it)
  {
    if (it->getDenotation() == DEVICE_QUEUE_DENOTATION_GRAPHICS_BITS)
      ctx->graphicsQueue = ctx->device->getQueueByFamily(it->queueFamilyIndex, 0u).value_or(nullptr);
    if (it->getDenotation() == DEVICE_QUEUE_DENOTATION_PRESENT_BITS)
      ctx->presentQueue = ctx->device->getQueueByFamily(it->queueFamilyIndex, 0u).value_or(nullptr);
  }
  if (!ctx->presentQueue) ctx->presentQueue = ctx->graphicsQueue;
  if (!ctx->transferQueue) ctx->transferQueue = ctx->graphicsQueue;
}

}