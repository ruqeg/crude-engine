#include <vulkan/vulkan.hpp>

module crude.graphics.image_memory_barrier;

import crude.graphics.image;
import crude.graphics.vulkan_utils;

namespace crude::graphics
{

Image_Memory_Barrier::Image_Memory_Barrier(core::Shared_Ptr<Image>  image,
                                           VkImageLayout            newLayout,
                                           Image_Subresource_Range  subresourceRange,
                                           core::uint32             srcQueueFamilyIndex,
                                           core::uint32             dstQueueFamilyIndex)
  :
  m_image(image)
{
  VkAccessFlags newSrcAccessMask;
  VkAccessFlags newDstAccessMask;

  if (image->getLayout() == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
  {
    newSrcAccessMask = 0;
    newDstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
  }
  else if (image->getLayout() == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
  {
    newSrcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    newDstAccessMask = VK_ACCESS_SHADER_READ_BIT;
  }
  else if (image->getLayout() == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
  {
    newSrcAccessMask = 0;
    newDstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
  }
  else 
  {
    vulkanHandleError("unsupported layout transition for image memory barrier!");
  }

  this->sType                = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  this->pNext                = nullptr;
  this->oldLayout            = image->getLayout();
  this->newLayout            = newLayout;
  this->srcAccessMask        = newSrcAccessMask;
  this->dstAccessMask        = newDstAccessMask;
  this->srcQueueFamilyIndex  = srcQueueFamilyIndex;
  this->dstQueueFamilyIndex  = dstQueueFamilyIndex;
  this->image                = m_image->getHandle();
  this->subresourceRange     = subresourceRange;
}

}