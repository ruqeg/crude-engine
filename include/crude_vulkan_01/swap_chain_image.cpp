#include "swap_chain_image.hpp"
#include "image.hpp"
#include "swap_chain.hpp"

namespace crude_vulkan_01 
{
  
  
SwapChainImageCreateInfo::SwapChainImageCreateInfo(std::shared_ptr<const Device>  device,
                                                   VkImage                        handle,
                                                   VkFormat                       format,
                                                   VkColorSpaceKHR                colorSpace,
                                                   VkExtent2D                     extent,
                                                   VkImageUsageFlags              usage)
  :
  ImageCreateInfo(device, handle, format, colorSpace, extent, usage)
{}

Swap_Chain_Image::Swap_Chain_Image(const SwapChainImageCreateInfo& createInfo)
  :
  Image(static_cast<ImageCreateInfo>(createInfo))
{}

Swap_Chain_Image::~Swap_Chain_Image()
{
  // !TODO
  m_handle = VK_NULL_HANDLE;
}

}
