#include <vulkan/vulkan.hpp>

module crude.graphics.swap_chain;

import crude.graphics.vulkan_utils;
import crude.graphics.device;
import crude.graphics.surface;
import crude.graphics.swap_chain_image;
import crude.graphics.fence;
import crude.graphics.semaphore;

namespace crude::graphics
{

Swap_Chain::Swap_Chain(core::shared_ptr<const Device>   device,
                       core::shared_ptr<const Surface>  surface,
                       const VkSurfaceFormatKHR&        surfaceFormat,
                       const VkExtent2D&                extent,
                       VkImageUsageFlags                imageUsage,
                       core::uint32                     minImageCount,
                       core::uint32                     imageArrayLayers,
                       VkSharingMode                    imageSharingMode,
                       const core::span<core::uint32>&  queueFamilyIndices,
                       VkSurfaceTransformFlagBitsKHR    preTransform,
                       VkCompositeAlphaFlagBitsKHR      compositeAlpha,
                       VkPresentModeKHR                 presentMode,
                       VkBool32                         clipped,
                       VkSwapchainCreateFlagsKHR        flags,
                       core::shared_ptr<Swap_Chain>     oldSwapchain)
  :
  m_device(device),
  m_surface(surface),
  m_imageUsage(imageUsage),
  m_surfaceFormat(surfaceFormat),
  m_extent(extent)
{
  core::vector<core::uint32> uniqueQueueFamilyIndices;
  std::unique_copy(queueFamilyIndices.begin(), queueFamilyIndices.end(), uniqueQueueFamilyIndices.begin());

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
  vkCreateInfo.imageSharingMode       = imageSharingMode;
  vkCreateInfo.queueFamilyIndexCount  = static_cast<core::uint32>(uniqueQueueFamilyIndices.size());
  vkCreateInfo.pQueueFamilyIndices    = uniqueQueueFamilyIndices.data();
  vkCreateInfo.preTransform           = preTransform;
  vkCreateInfo.compositeAlpha         = compositeAlpha;
  vkCreateInfo.presentMode            = presentMode;
  vkCreateInfo.clipped                = clipped;
  vkCreateInfo.oldSwapchain           = oldSwapchain ? oldSwapchain->getHandle() : VK_NULL_HANDLE;

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

Swap_Chain_Next_Image Swap_Chain::acquireNextImage(const core::Optional<core::shared_ptr<Semaphore>>&  semaphore,
                                                   const core::Optional<core::shared_ptr<Fence>>&      fence,
                                                   core::uint64                                        timeout)
{
  core::uint32 imageIndex;
  VkResult result = vkAcquireNextImageKHR(
    m_device->getHandle(),
    m_handle,
    timeout,
    semaphore ? semaphore->getHandle() : VK_NULL_HANDLE,
    fence ? fence->getHandle() : VK_NULL_HANDLE,
    &imageIndex);

  const Swap_Chain_Next_Image nextImage(imageIndex, result);
  return nextImage;
}

const VkImageUsageFlags Swap_Chain::getImageUsage() const
{
  return m_imageUsage;
}

const VkSurfaceFormatKHR Swap_Chain::getSurfaceFormat() const
{
  return m_surfaceFormat;
}

const VkExtent2D Swap_Chain::getExtent() const
{
  return m_extent;
}

Swap_Chain_Next_Image::Swap_Chain_Next_Image(core::uint32 imageIndex, VkResult result)
  :
  m_imageIndex(imageIndex),
  m_result(result)
{}

core::Optional<core::uint32> Swap_Chain_Next_Image::getImageIndex() const
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
