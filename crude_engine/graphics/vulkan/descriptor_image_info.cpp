#include <vulkan/vulkan.hpp>

module crude_engine.graphics.vulkan.descriptor_image_info;

import crude_engine.graphics.vulkan.sampler;
import crude_engine.graphics.vulkan.image_view;


namespace crude_engine
{

Descriptor_Image_Info::Descriptor_Image_Info(Shared_Ptr<Sampler>     sampler,
                                             Shared_Ptr<Image_View>  imageView,
                                             VkImageLayout                imageLayout)
  :
  m_sampler(sampler),
  m_imageView(imageView)
{
  this->sampler      = m_sampler->getHandle();
  this->imageView    = m_imageView->getHandle();
  this->imageLayout  = imageLayout;
}

}
