#include "swap_chain.hpp"
#include "core.hpp"
#include "device.hpp"
#include "surface.hpp"
#include <vulkan/vulkan_core.h>


namespace crude_vulkan_01
{

SwapChainCreateInfo::SwapChainCreateInfo(std::shared_ptr<const Device>   device,
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
  
Swap_Chain::Swap_Chain(const SwapChainCreateInfo& createInfo)
  :
  m_device(createInfo.device),
  m_surface(createInfo.surface)
{
  VkSwapchainCreateInfoKHR vkCreateInfo{};
  vkCreateInfo.sType                  = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
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

  VkResult result = vkCreateSwapchainKHR(
    CRUDE_VULKAN_01_HANDLE(m_device), 
    &vkCreateInfo, 
    nullptr, 
    &m_handle);

  CRUDE_VULKAN_01_HANDLE_RESULT(result, "failed to create swapchain");
}

Swap_Chain::~Swap_Chain()
{
  vkDestroySwapchainKHR(
    CRUDE_VULKAN_01_HANDLE(m_device), 
    m_handle, 
    nullptr);
}

}
