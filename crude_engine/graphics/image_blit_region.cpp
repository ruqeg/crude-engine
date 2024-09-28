#include <vulkan/vulkan.h>

module crude.graphics.image_blit_region;

import crude.graphics.image;

namespace crude::graphics
{

Image_Blit_Region::Image_Blit_Region(core::shared_ptr<Image> srcImage, core::shared_ptr<Image> dstImage)
  : m_srcImage(srcImage)
  , m_dstImage(dstImage)
{
  this->srcOffsets[0]   = VkOffset3D{ 0, 0, 0 };
  this->srcOffsets[1]   = VkOffset3D{ static_cast<core::int32>(srcImage->getWidth()), static_cast<core::int32>(srcImage->getHeight()), static_cast<core::int32>(srcImage->getDepth()) };
  this->srcSubresource  = Image_Subresource_Layers(srcImage);
  this->dstOffsets[0]   = VkOffset3D{ 0, 0, 0 };
  this->dstOffsets[1]   = VkOffset3D{ static_cast<core::int32>(dstImage->getWidth()), static_cast<core::int32>(dstImage->getHeight()), static_cast<core::int32>(dstImage->getDepth()) };
  this->dstSubresource  = Image_Subresource_Layers(dstImage);
}

core::shared_ptr<Image> Image_Blit_Region::getSourceImage() const
{
  return m_srcImage;
}

core::shared_ptr<Image> Image_Blit_Region::getDestinationImage() const
{
  return m_dstImage;
}

Image_Blit_Region::Image_Blit_Region(const Initialize& initialize)
  : m_srcImage(initialize.srcImage)
  , m_dstImage(initialize.dstImage)
{
  this->srcOffsets[0]   = initialize.srcOffsets[0];
  this->srcOffsets[1]   = initialize.srcOffsets[1];
  this->srcSubresource  = initialize.srcLayers;
  this->dstOffsets[0]   = initialize.dstOffsets[0];
  this->dstOffsets[1]   = initialize.dstOffsets[1];
  this->dstSubresource  = initialize.dstLayers;
}


}