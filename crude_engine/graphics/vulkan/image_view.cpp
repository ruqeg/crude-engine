#include <vulkan/vulkan.hpp>

module crude.gfx.vk.image_view;

import crude.gfx.vk.vulkan_utils;
import crude.gfx.vk.image;
import crude.gfx.vk.device;

namespace crude::gfx::vk
{

Image_View::Image_View(core::shared_ptr<Image> image)
  : Image_View(image, Image_Subresource_Range(image))
{}
  
Image_View::Image_View(core::shared_ptr<Image>         image,
                       const Image_Subresource_Range&  subresourceRange,
                       const VkComponentMapping&       components)
  : Image_View(image, image->getFormat(), subresourceRange, components)
{}

Image_View::Image_View(core::shared_ptr<Image>         image,
                       VkFormat                        format,
                       const Image_Subresource_Range&  subresourceRange,
                       const VkComponentMapping&       components)
  : m_image(image)
  , m_subresourceRange(subresourceRange)
{
  VkImageViewCreateInfo vkCreateInfo{};
  vkCreateInfo.sType                            = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  vkCreateInfo.pNext                            = nullptr;
  vkCreateInfo.flags                            = 0u;

  vkCreateInfo.viewType                         = imageToViewType(m_image->getType(), m_image->getFlags(), m_image->getFlags());
  vkCreateInfo.image                            = m_image->getHandle();
  vkCreateInfo.components                       = components;
  vkCreateInfo.format                           = format;
  vkCreateInfo.subresourceRange                 = m_subresourceRange;

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

core::shared_ptr<Image> Image_View::getImage()
{
    return m_image;
}
  
VkImageViewType Image_View::imageToViewType(VkImageType imageType, VkImageCreateFlags imageFlags, core::uint32 arrayLayersCount)
{
  switch (imageType) 
  {
    case VK_IMAGE_TYPE_1D:
      return VK_IMAGE_VIEW_TYPE_1D;
    case VK_IMAGE_TYPE_2D:
      if (imageFlags & VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT)
        return (arrayLayersCount > 6) ? VK_IMAGE_VIEW_TYPE_CUBE_ARRAY : VK_IMAGE_VIEW_TYPE_CUBE;
      return VK_IMAGE_VIEW_TYPE_2D;
    case VK_IMAGE_TYPE_3D:
      return VK_IMAGE_VIEW_TYPE_3D;
  }
  return VK_IMAGE_VIEW_TYPE_MAX_ENUM;
}


}
