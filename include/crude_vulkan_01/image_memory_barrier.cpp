#include "image_memory_barrier.hpp"
#include "image.hpp"

namespace crude_vulkan_01
{

Image_Memory_Barrier::Image_Memory_Barrier(std::shared_ptr<Image>   image,
                                           VkImageLayout            newLayout,
                                           Image_Subresource_Range  subresourceRange,
                                           uint32                   srcQueueFamilyIndex,
                                           uint32                   dstQueueFamilyIndex)
{
  m_image = image;

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
    CRUDE_VULKAN_01_HANDLE_RESULT(VK_FALSE, "unsupported layout transition for image memory barrier!");
  }

  this->sType                            = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  this->pNext                            = nullptr;
  this->oldLayout                        = image->getLayout();
  this->newLayout                        = newLayout;
  this->srcAccessMask                    = newSrcAccessMask;
  this->dstAccessMask                    = newDstAccessMask;
  this->srcQueueFamilyIndex              = srcQueueFamilyIndex;
  this->dstQueueFamilyIndex              = dstQueueFamilyIndex;
  this->image                            = CRUDE_VULKAN_01_HANDLE(m_image);
  this->subresourceRange                 = subresourceRange;
}

}