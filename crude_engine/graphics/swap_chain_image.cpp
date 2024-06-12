#include <vulkan/vulkan.hpp>

module crude.graphics.swap_chain_image;

import crude.graphics.device;

namespace crude::graphics
{

Swap_Chain_Image::Swap_Chain_Image(core::shared_ptr<const Device>  device,
                                   VkImage                         handle,
                                   VkFormat                        format,
                                   VkColorSpaceKHR                 colorSpace,
                                   VkExtent2D                      extent,
                                   VkImageUsageFlags               usage)

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
