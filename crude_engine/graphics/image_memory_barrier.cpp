#include <vulkan/vulkan.hpp>

module crude.graphics.image_memory_barrier;

import crude.graphics.image;
import crude.graphics.vulkan_utils;
import crude.core.logger;

namespace crude::graphics
{

Image_Memory_Barrier::Image_Memory_Barrier(core::shared_ptr<Image>  image,
                                           VkImageLayout            newLayout,
                                           Image_Subresource_Range  subresourceRange,
                                           core::uint32             srcQueueFamilyIndex,
                                           core::uint32             dstQueueFamilyIndex)
  :
  m_image(image)
{

  this->sType                = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  this->pNext                = nullptr;
  this->oldLayout            = image->getLayout();
  this->newLayout            = newLayout;
  this->srcAccessMask        = 0u;
  this->dstAccessMask        = 0u;
  this->srcQueueFamilyIndex  = srcQueueFamilyIndex;
  this->dstQueueFamilyIndex  = dstQueueFamilyIndex;
  this->image                = m_image->getHandle();
  this->subresourceRange     = subresourceRange;

  switch (oldLayout)
  {
  case VK_IMAGE_LAYOUT_UNDEFINED:
    break;
  case VK_IMAGE_LAYOUT_GENERAL:
    srcAccessMask = VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT;
    break;
  case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
    srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    break;
  case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
    srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    break;
  case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
    srcAccessMask = VK_ACCESS_NONE_KHR;
    break;
  case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
    srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    break;
  case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
    srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    break;
  default:
    core::logError(core::Debug::Channel::Graphics, "Unsupported layout transition for image memory barrier!");
  }
  switch (newLayout)
  {
  case VK_IMAGE_LAYOUT_GENERAL:
    dstAccessMask = VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT;
    break;
  case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
    dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    break;
  case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
    dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    break;
  case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:
    dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
  case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
    if (image->getUsage() & VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT)
      dstAccessMask |= VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
    else if (image->getUsage() & VK_IMAGE_USAGE_SAMPLED_BIT)
      dstAccessMask |= VK_ACCESS_SHADER_READ_BIT;
    break;
  case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
    dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    break;
  case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
    dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    break;
  default:
    core::logError(core::Debug::Channel::Graphics, "Unsupported layout transition for image memory barrier!");
  }
}

}