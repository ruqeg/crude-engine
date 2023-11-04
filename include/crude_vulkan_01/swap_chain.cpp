#include "swap_chain.hpp"
#include "device.hpp"
#include "surface.hpp"
#include "swap_chain_image.hpp"


namespace crude_vulkan_01
{

Swap_Chain_Create_Info::Swap_Chain_Create_Info(std::shared_ptr<const Device>   device,
                                               std::shared_ptr<const Surface>  surface,
                                               const VkSurfaceFormatKHR&       surfaceFormat,
                                               const VkExtent2D&               extent,
                                               VkImageUsageFlags               imageUsage,
                                               uint32_t                        minImageCount,
                                               uint32_t                        imageArrayLayers,
                                               VkSharingMode                   imageSharingMode,
                                               const std::vector<uint32>&      queueFamilyIndices,
                                               VkSurfaceTransformFlagBitsKHR   preTransform,
                                               VkCompositeAlphaFlagBitsKHR     compositeAlpha,
                                               VkPresentModeKHR                presentMode,
                                               VkBool32                        clipped,
                                               VkSwapchainCreateFlagsKHR       flags,
                                               std::shared_ptr<Swap_Chain>     oldSwapchain)
  :
  device(device),
  surface(surface),
  surfaceFormat(surfaceFormat),
  extent(extent),
  imageUsage(imageUsage),
  minImageCount(minImageCount),
  imageArrayLayers(imageArrayLayers),
  imageSharingMode(imageSharingMode),
  queueFamilyIndices(queueFamilyIndices),
  preTransform(preTransform),
  compositeAlpha(compositeAlpha),
  presentMode(presentMode),
  clipped(clipped),
  flags(flags),
  oldSwapchain(oldSwapchain)
{}
  
Swap_Chain::Swap_Chain(const Swap_Chain_Create_Info& createInfo)
  :
  m_device(createInfo.device),
  m_surface(createInfo.surface),
  m_imageUsage(createInfo.imageUsage),
  m_surfaceFormat(createInfo.surfaceFormat),
  m_extent(createInfo.extent)
{
  VkSwapchainCreateInfoKHR vkCreateInfo{};
  vkCreateInfo.sType                  = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  vkCreateInfo.pNext                  = nullptr;

  vkCreateInfo.flags                  = createInfo.flags;
  vkCreateInfo.surface                = CRUDE_VULKAN_01_HANDLE(m_surface);
  vkCreateInfo.minImageCount          = createInfo.minImageCount;
  vkCreateInfo.imageFormat            = createInfo.surfaceFormat.format;
  vkCreateInfo.imageColorSpace        = createInfo.surfaceFormat.colorSpace;
  vkCreateInfo.imageExtent            = createInfo.extent;
  vkCreateInfo.imageArrayLayers       = createInfo.imageArrayLayers;
  vkCreateInfo.imageUsage             = createInfo.imageUsage;
  vkCreateInfo.imageSharingMode       = createInfo.imageSharingMode;
  vkCreateInfo.queueFamilyIndexCount  = static_cast<uint32>(createInfo.queueFamilyIndices.size());
  vkCreateInfo.pQueueFamilyIndices    = createInfo.queueFamilyIndices.data();
  vkCreateInfo.preTransform           = createInfo.preTransform;
  vkCreateInfo.compositeAlpha         = createInfo.compositeAlpha;
  vkCreateInfo.presentMode            = createInfo.presentMode;
  vkCreateInfo.clipped                = createInfo.clipped;
  if (createInfo.oldSwapchain)
  {
    vkCreateInfo.oldSwapchain         = CRUDE_VULKAN_01_HANDLE(createInfo.oldSwapchain);
  }
  else 
  {
    vkCreateInfo.oldSwapchain         = VK_NULL_HANDLE;
  }

  VkResult result = vkCreateSwapchainKHR(CRUDE_VULKAN_01_HANDLE(m_device), &vkCreateInfo, nullptr, &m_handle);
  CRUDE_VULKAN_01_HANDLE_RESULT(result, "failed to create swapchain");
}

Swap_Chain::~Swap_Chain()
{
  vkDestroySwapchainKHR(CRUDE_VULKAN_01_HANDLE(m_device), m_handle, nullptr);
}
  
const std::vector<std::shared_ptr<Swap_Chain_Image>>& Swap_Chain::getSwapchainImages()
{
  if (!m_swapChainImages.empty())
  {
    return m_swapChainImages;
  }

  uint32 imageCount = 0u;
  vkGetSwapchainImagesKHR(CRUDE_VULKAN_01_HANDLE(m_device), m_handle, &imageCount, nullptr);
  
  if (imageCount == 0u)
  {
    return m_swapChainImages;
  }

  std::vector<VkImage> vkSwapchainImages(imageCount);
  vkGetSwapchainImagesKHR(CRUDE_VULKAN_01_HANDLE(m_device), m_handle, &imageCount, vkSwapchainImages.data());

  m_swapChainImages.resize(imageCount);
  for (uint32 i = 0; i < imageCount; ++i)
  {
    m_swapChainImages[i] = std::make_shared<Swap_Chain_Image>(Swap_Chain_Image_Create_Info(
      m_device,
      vkSwapchainImages[i],
      m_surfaceFormat.format,
      m_surfaceFormat.colorSpace,
      m_extent,
      m_imageUsage));
  }

  return m_swapChainImages;
}

}
