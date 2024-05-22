#include <vulkan/vulkan.hpp>

module crude.graphics.vulkan.descriptor_image_info;


namespace crude::graphics
{

Descriptor_Image_Info::Descriptor_Image_Info(core::Shared_Ptr<Sampler>     sampler,
                                             core::Shared_Ptr<Image_View>  imageView,
                                             VkImageLayout                 imageLayout)
  :
  m_sampler(sampler),
  m_imageView(imageView)
{
  this->sampler      = m_sampler->getHandle();
  this->imageView    = m_imageView->getHandle();
  this->imageLayout  = imageLayout;
}

}
