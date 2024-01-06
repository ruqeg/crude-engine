#include <graphics/vulkan/image_subresource_range.hpp>
#include <graphics/vulkan/image.hpp>
#include <graphics/vulkan/format.hpp>
#include <core/debug/assert.hpp>

namespace crude_engine
{

Image_Subresource_Range::Image_Subresource_Range(Image*  pImage,
                                                 uint32  baseMipLevel,
                                                 uint32  levelCount,
                                                 uint32  baseArrayLayer,
                                                 uint32  layerCount)
{
  CRUDE_ASSERT(pImage);

  this->baseMipLevel    = baseMipLevel;
  this->levelCount      = levelCount;
  this->baseArrayLayer  = baseArrayLayer;
  this->layerCount      = layerCount;

  Format format(pImage->getFormat());

  if (format.depth())              this->aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
  else if (format.stencil())       this->aspectMask = VK_IMAGE_ASPECT_STENCIL_BIT;
  else if (format.depthStencil())  this->aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
  else                             this->aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
}

}