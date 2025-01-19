#include <vulkan/vulkan.hpp>

module crude.gfx.vk.swap_chain;

import crude.gfx.vk.vulkan_utils;
import crude.gfx.vk.device;
import crude.gfx.vk.surface;
import crude.gfx.vk.swap_chain_image;
import crude.gfx.vk.fence;
import crude.gfx.vk.semaphore;
import crude.gfx.vk.utils;

namespace crude::gfx::vk
{

Swap_Chain::Swap_Chain(core::shared_ptr<const Device>   device,
                       core::shared_ptr<const Surface>  surface,
                       const VkSurfaceFormatKHR&        surfaceFormat,
                       const VkExtent2D&                extent,
                       VkImageUsageFlags                imageUsage,
                       core::uint32                     minImageCount,
                       core::uint32                     imageArrayLayers,
                       VkSurfaceTransformFlagBitsKHR    preTransform,
                       VkCompositeAlphaFlagBitsKHR      compositeAlpha,
                       VkPresentModeKHR                 presentMode,
                       VkBool32                         clipped,
                       VkSwapchainCreateFlagsKHR        flags,
                       core::shared_ptr<Swap_Chain>     oldSwapchain)
  : Swap_Chain(device, surface, surfaceFormat, extent, imageUsage, minImageCount, imageArrayLayers, {},
    preTransform, compositeAlpha, presentMode, clipped, flags, oldSwapchain) 
{}

Swap_Chain::Swap_Chain(core::shared_ptr<const Device>   device,
                       core::shared_ptr<const Surface>  surface,
                       const VkSurfaceFormatKHR&        surfaceFormat,
                       const VkExtent2D&                extent,
                       VkImageUsageFlags                imageUsage,
                       core::uint32                     minImageCount,
                       core::uint32                     imageArrayLayers,
                       core::span<const core::uint32>   queueFamilyIndices,
                       VkSurfaceTransformFlagBitsKHR    preTransform,
                       VkCompositeAlphaFlagBitsKHR      compositeAlpha,
                       VkPresentModeKHR                 presentMode,
                       VkBool32                         clipped,
                       VkSwapchainCreateFlagsKHR        flags,
                       core::shared_ptr<Swap_Chain>     oldSwapchain)
  : m_device(device)
  , m_surface(surface)
  , m_imageUsage(imageUsage)
  , m_surfaceFormat(surfaceFormat)
  , m_extent(extent)
  , m_presentMode(presentMode)
{
  // !TODO
  core::vector<core::uint32> uniqueQueueFamilyIndices(queueFamilyIndices.begin(), queueFamilyIndices.end());
  uniqueQueueFamilyIndices.erase(std::unique(uniqueQueueFamilyIndices.begin(), uniqueQueueFamilyIndices.end()), uniqueQueueFamilyIndices.end());

  VkSwapchainCreateInfoKHR vkCreateInfo{};
  vkCreateInfo.sType                  = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  vkCreateInfo.pNext                  = nullptr;

  vkCreateInfo.flags                  = flags;
  vkCreateInfo.surface                = m_surface->getHandle();
  vkCreateInfo.minImageCount          = minImageCount;
  vkCreateInfo.imageFormat            = surfaceFormat.format;
  vkCreateInfo.imageColorSpace        = surfaceFormat.colorSpace;
  vkCreateInfo.imageExtent            = extent;
  vkCreateInfo.imageArrayLayers       = imageArrayLayers;
  vkCreateInfo.imageUsage             = imageUsage;
  vkCreateInfo.imageSharingMode       = uniqueQueueFamilyIndices.size() > 1 ? VK_SHARING_MODE_CONCURRENT : VK_SHARING_MODE_EXCLUSIVE;
  vkCreateInfo.queueFamilyIndexCount  = static_cast<core::uint32>(uniqueQueueFamilyIndices.size());
  vkCreateInfo.pQueueFamilyIndices    = uniqueQueueFamilyIndices.data();
  vkCreateInfo.preTransform           = preTransform;
  vkCreateInfo.compositeAlpha         = compositeAlpha;
  vkCreateInfo.presentMode            = presentMode;
  vkCreateInfo.clipped                = clipped;
  vkCreateInfo.oldSwapchain           = getObjectHandle<VkSwapchainKHR>(oldSwapchain);

  VkResult result = vkCreateSwapchainKHR(m_device->getHandle(), &vkCreateInfo, getPVkAllocationCallbacks(), &m_handle);
  vulkanHandleResult(result, "failed to create swapchain");
}

Swap_Chain::~Swap_Chain()
{
  vkDestroySwapchainKHR(m_device->getHandle(), m_handle, getPVkAllocationCallbacks());
}
  
const core::vector<core::shared_ptr<Swap_Chain_Image>>& Swap_Chain::getSwapchainImages()
{
  if (!m_swapChainImages.empty())
  {
    return m_swapChainImages;
  }

  core::uint32 imageCount = 0u;
  vkGetSwapchainImagesKHR(m_device->getHandle(), m_handle, &imageCount, nullptr);
  
  if (imageCount == 0u)
  {
    return m_swapChainImages;
  }

  core::vector<VkImage> vkSwapchainImages(imageCount);
  vkGetSwapchainImagesKHR(m_device->getHandle(), m_handle, &imageCount, vkSwapchainImages.data());

  m_swapChainImages.resize(imageCount);
  for (core::uint32 i = 0; i < imageCount; ++i)
  {
    m_swapChainImages[i] = core::allocateShared<Swap_Chain_Image>(
      m_device,
      vkSwapchainImages[i],
      m_surfaceFormat.format,
      m_surfaceFormat.colorSpace,
      m_extent);
  }

  return m_swapChainImages;
}

Swap_Chain_Next_Image Swap_Chain::acquireNextImage(const core::optional<core::shared_ptr<Semaphore>>&  semaphore,
                                                   const core::optional<core::shared_ptr<Fence>>&      fence,
                                                   core::uint64                                        timeout)
{
  core::uint32 imageIndex;
  VkResult result = vkAcquireNextImageKHR(
    m_device->getHandle(),
    m_handle,
    timeout,
    getOptionalObjectHandle<VkSemaphore>(semaphore),
    getOptionalObjectHandle<VkFence>(fence),
    &imageIndex);

  const Swap_Chain_Next_Image nextImage(imageIndex, result);
  return nextImage;
}

core::shared_ptr<const Device> Swap_Chain::getDevice()
{
  return m_device;
}

Swap_Chain_Next_Image::Swap_Chain_Next_Image(core::uint32 imageIndex, VkResult result)
  :
  m_imageIndex(imageIndex),
  m_result(result)
{}

core::optional<core::uint32> Swap_Chain_Next_Image::getImageIndex() const
{
  if (failedAcquire())
    return core::nullopt;
  return m_imageIndex;
}

VkResult Swap_Chain_Next_Image::getResult() const
{
  return m_result;
}

bool Swap_Chain_Next_Image::outOfDate() const
{
  return m_result == VK_ERROR_OUT_OF_DATE_KHR;
}

bool Swap_Chain_Next_Image::failedAcquire() const
{
  return (m_result != VK_SUCCESS) && (m_result != VK_SUBOPTIMAL_KHR);
}

}
