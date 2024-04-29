#include "swap_chain.hpp"
#include "device.hpp"
#include "surface.hpp"
#include "swap_chain_image.hpp"
#include "fence.hpp"
#include "semaphore.hpp"

#include <graphics/vulkan/swap_chain.hpp>
#include <graphics/vulkan/device.hpp>
#include <graphics/vulkan/surface.hpp>
#include <graphics/vulkan/swap_chain_image.hpp>
#include <graphics/vulkan/fence.hpp>
#include <graphics/vulkan/semaphore.hpp>

namespace crude_engine
{

Swap_Chain::Swap_Chain(Shared_Ptr<const Device>       device,
                       Shared_Ptr<const Surface>      surface,
                       const VkSurfaceFormatKHR&      surfaceFormat,
                       const VkExtent2D&              extent,
                       VkImageUsageFlags              imageUsage,
                       uint32_t                       minImageCount,
                       uint32_t                       imageArrayLayers,
                       VkSharingMode                  imageSharingMode,
                       const span<uint32>&            queueFamilyIndices,
                       VkSurfaceTransformFlagBitsKHR  preTransform,
                       VkCompositeAlphaFlagBitsKHR    compositeAlpha,
                       VkPresentModeKHR               presentMode,
                       VkBool32                       clipped,
                       VkSwapchainCreateFlagsKHR      flags,
                       Shared_Ptr<Swap_Chain>         oldSwapchain)
  :
  m_device(device),
  m_surface(surface),
  m_imageUsage(imageUsage),
  m_surfaceFormat(surfaceFormat),
  m_extent(extent)
{
  VkSwapchainCreateInfoKHR vkCreateInfo{};
  vkCreateInfo.sType                  = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  vkCreateInfo.pNext                  = nullptr;

  vkCreateInfo.flags                  = flags;
  vkCreateInfo.surface                = CRUDE_OBJECT_HANDLE(m_surface);
  vkCreateInfo.minImageCount          = minImageCount;
  vkCreateInfo.imageFormat            = surfaceFormat.format;
  vkCreateInfo.imageColorSpace        = surfaceFormat.colorSpace;
  vkCreateInfo.imageExtent            = extent;
  vkCreateInfo.imageArrayLayers       = imageArrayLayers;
  vkCreateInfo.imageUsage             = imageUsage;
  vkCreateInfo.imageSharingMode       = imageSharingMode;
  vkCreateInfo.queueFamilyIndexCount  = static_cast<uint32>(queueFamilyIndices.size());
  vkCreateInfo.pQueueFamilyIndices    = queueFamilyIndices.data();
  vkCreateInfo.preTransform           = preTransform;
  vkCreateInfo.compositeAlpha         = compositeAlpha;
  vkCreateInfo.presentMode            = presentMode;
  vkCreateInfo.clipped                = clipped;
  vkCreateInfo.oldSwapchain           = oldSwapchain ? CRUDE_OBJECT_HANDLE(oldSwapchain) : VK_NULL_HANDLE;

  VkResult result = vkCreateSwapchainKHR(CRUDE_OBJECT_HANDLE(m_device), &vkCreateInfo, getPVkAllocationCallbacks(), &m_handle);
  CRUDE_VULKAN_HANDLE_RESULT(result, "failed to create swapchain");
}

Swap_Chain::~Swap_Chain()
{
  vkDestroySwapchainKHR(CRUDE_OBJECT_HANDLE(m_device), m_handle, getPVkAllocationCallbacks());
}
  
const vector<Shared_Ptr<Swap_Chain_Image>>& Swap_Chain::getSwapchainImages()
{
  if (!m_swapChainImages.empty())
  {
    return m_swapChainImages;
  }

  uint32 imageCount = 0u;
  vkGetSwapchainImagesKHR(CRUDE_OBJECT_HANDLE(m_device), m_handle, &imageCount, nullptr);
  
  if (imageCount == 0u)
  {
    return m_swapChainImages;
  }

  vector<VkImage> vkSwapchainImages(imageCount);
  vkGetSwapchainImagesKHR(CRUDE_OBJECT_HANDLE(m_device), m_handle, &imageCount, vkSwapchainImages.data());

  m_swapChainImages.resize(imageCount);
  for (uint32 i = 0; i < imageCount; ++i)
  {
    m_swapChainImages[i] = makeShared<Swap_Chain_Image>(
      m_device,
      vkSwapchainImages[i],
      m_surfaceFormat.format,
      m_surfaceFormat.colorSpace,
      m_extent,
      m_imageUsage);
  }

  return m_swapChainImages;
}

Swap_Chain_Next_Image Swap_Chain::acquireNextImage(const Optional<Shared_Ptr<Semaphore>>&  semaphore,
                                                   const Optional<Shared_Ptr<Fence>>&      fence,
                                                   uint64                                  timeout)
{
  uint32 imageIndex;
  VkResult result = vkAcquireNextImageKHR(
    CRUDE_OBJECT_HANDLE(m_device),
    m_handle,
    timeout,
    semaphore ? CRUDE_OBJECT_HANDLE(semaphore) : VK_NULL_HANDLE,
    fence ? CRUDE_OBJECT_HANDLE(fence) : VK_NULL_HANDLE,
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

Swap_Chain_Next_Image::Swap_Chain_Next_Image(uint32 imageIndex, VkResult result)
  :
  m_imageIndex(imageIndex),
  m_result(result)
{}

Optional<uint32> Swap_Chain_Next_Image::getImageIndex() const
{
  if (failedAcquire())
    return nullopt;
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
