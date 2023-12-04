#include "image_subresource_range.hpp"
#include "image.hpp"
#include "format.hpp"

namespace crude_engine
{

Image_Subresource_Range::Image_Subresource_Range(std::shared_ptr<Image>  image,
                                                 uint32                  baseMipLevel,
                                                 uint32                  levelCount,
                                                 uint32                  baseArrayLayer,
                                                 uint32                  layerCount)
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