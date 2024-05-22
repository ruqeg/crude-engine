#include <vulkan/vulkan.hpp>

module crude.graphics.vulkan.image_subresource_range;

import crude.graphics.vulkan.image;
import crude.graphics.vulkan.format;

namespace crude::graphics
{

Image_Subresource_Range::Image_Subresource_Range(core::Shared_Ptr<Image>  image,
                                                 core::uint32             baseMipLevel,
                                                 core::uint32             levelCount,
                                                 core::uint32             baseArrayLayer,
                                                 core::uint32             layerCount)
{
  this->baseMipLevel    = baseMipLevel;
  this->levelCount      = levelCount;
  this->baseArrayLayer  = baseArrayLayer;
  this->layerCount      = layerCount;

  Format format(image->getFormat());

  if (format.depth())              this->aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
  else if (format.stencil())       this->aspectMask = VK_IMAGE_ASPECT_STENCIL_BIT;
  else if (format.depthStencil())  this->aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
  else                             this->aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
}

}