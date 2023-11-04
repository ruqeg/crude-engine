#include "swap_chain_image.hpp"
#include "image.hpp"
#include "swap_chain.hpp"

namespace crude_vulkan_01 
{
  
  
Swap_Chain_Image_Create_Info::Swap_Chain_Image_Create_Info(std::shared_ptr<const Device>  device,
                                                           VkImage                        handle,
                                                           VkFormat                       format,
                                                           VkColorSpaceKHR                colorSpace,
                                                           VkExtent2D                     extent,
                                                           VkImageUsageFlags              usage)
  :
  Image_Create_Info(device, handle, format, colorSpace, extent, usage, VK_IMAGE_TYPE_2D)
{}

Swap_Chain_Image::Swap_Chain_Image(const Swap_Chain_Image_Create_Info& createInfo)
  :
  Image(static_cast<Image_Create_Info>(createInfo))
{}

Swap_Chain_Image::~Swap_Chain_Image()
{
  // !TODO
  m_handle = VK_NULL_HANDLE;
}

}
