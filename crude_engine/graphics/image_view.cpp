#include <vulkan/vulkan.hpp>

module crude.graphics.image_view;

import crude.graphics.vulkan_utils;
import crude.graphics.image;
import crude.graphics.device;

namespace crude::graphics
{
  
Image_View::Image_View(core::shared_ptr<const Image>   image,
                       const Image_Subresource_Range&  subresourceRange,
                       const VkComponentMapping&       components)
  :
  Image_View(image, image->getFormat(), subresourceRange, components)
{}

Image_View::Image_View(core::shared_ptr<const Image>   image,
                       VkFormat                        format,
                       const Image_Subresource_Range&  subresourceRange,
                       const VkComponentMapping&       components)
  :
  m_image(image)
{
  VkImageViewCreateInfo vkCreateInfo{};
  vkCreateInfo.sType                            = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  vkCreateInfo.pNext                            = nullptr;
  vkCreateInfo.flags                            = 0u;

  vkCreateInfo.viewType                         = imageToViewType(m_image->getType());
  vkCreateInfo.image                            = m_image->getHandle();
  vkCreateInfo.components                       = components;
  vkCreateInfo.format                           = format;
  vkCreateInfo.subresourceRange                 = subresourceRange;

  VkResult result = vkCreateImageView(m_image->getDevice()->getHandle(), &vkCreateInfo, getPVkAllocationCallbacks(), &m_handle);
  vulkanHandleResult(result, "failed to create image view");
}

Image_View::~Image_View()
{
  vkDestroyImageView(m_image->getDevice()->getHandle(), m_handle, getPVkAllocationCallbacks());
}

core::shared_ptr<const Image> Image_View::getImage() const
{
    return m_image;
}
  
VkImageViewType Image_View::imageToViewType(VkImageType imageType)
{
  switch (imageType) 
  {
    case VK_IMAGE_TYPE_1D:  return VK_IMAGE_VIEW_TYPE_1D;
    case VK_IMAGE_TYPE_2D:  return VK_IMAGE_VIEW_TYPE_2D;
    case VK_IMAGE_TYPE_3D:  return VK_IMAGE_VIEW_TYPE_3D;
    // !TODO
  }
  return VK_IMAGE_VIEW_TYPE_MAX_ENUM;
}


}
