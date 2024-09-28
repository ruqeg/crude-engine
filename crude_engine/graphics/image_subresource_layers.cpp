#include <vulkan/vulkan.hpp>

module crude.graphics.image_subresource_layers;

import crude.graphics.image;
import crude.graphics.format;

namespace crude::graphics
{

Image_Subresource_Layers::Image_Subresource_Layers(core::shared_ptr<const Image> image)
  :
  Image_Subresource_Layers(image, 0u, 0u, image->getArrayLayersCount())
{}

Image_Subresource_Layers::Image_Subresource_Layers(core::shared_ptr<const Image>  image,
                                                   core::uint32                   mipLevel,
                                                   core::uint32                   baseArrayLayer,
                                                   core::uint32                   layerCount)
{
  this->mipLevel       = mipLevel;
  this->baseArrayLayer = baseArrayLayer;
  this->layerCount     = layerCount;

  const Format format(image->getFormat());
  if (format.depth())              this->aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
  else if (format.stencil())       this->aspectMask = VK_IMAGE_ASPECT_STENCIL_BIT;
  else if (format.depthStencil())  this->aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
  else                             this->aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
}

}