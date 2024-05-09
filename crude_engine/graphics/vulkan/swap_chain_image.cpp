#include <vulkan/vulkan.hpp>

module crude_engine.graphics.vulkan.swap_chain_image;

namespace crude_engine 
{

Swap_Chain_Image::Swap_Chain_Image(Shared_Ptr<const Device>  device,
                                   VkImage                   handle,
                                   VkFormat                  format,
                                   VkColorSpaceKHR           colorSpace,
                                   VkExtent2D                extent,
                                   VkImageUsageFlags         usage)

  :
  m_colorSpace(colorSpace),
  Image(device, handle, format, VkExtent3D{ extent.width, extent.height, 1u }, usage, VK_IMAGE_TYPE_2D)
{}

Swap_Chain_Image::~Swap_Chain_Image()
{
  // !TODO
  m_handle = VK_NULL_HANDLE;
}

}
