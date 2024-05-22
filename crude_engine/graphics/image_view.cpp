#include <vulkan/vulkan.hpp>

module crude.graphics.vulkan.image_view;

import crude.graphics.vulkan.vulkan_utils;
import crude.graphics.vulkan.image;
import crude.graphics.vulkan.device;

namespace crude::graphics
{
  

Image_View::Image_View(core::Shared_Ptr<const Device>  device,
                       core::Shared_Ptr<const Image>   image,
                       VkFormat                        format,
                       const Image_Subresource_Range&  subresourceRange,
                       const VkComponentMapping&       components)
  :
  m_device(device),
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

  VkResult result = vkCreateImageView(m_device->getHandle(), &vkCreateInfo, getPVkAllocationCallbacks(), &m_handle);
  vulkanHandleResult(result, "failed to create image view");
}

Image_View::~Image_View()
{
  vkDestroyImageView(m_device->getHandle(), m_handle, getPVkAllocationCallbacks());
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
