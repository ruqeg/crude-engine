#include <vulkan/vulkan.hpp>

module crude.graphics.descriptor_image_info;


namespace crude::graphics
{

Descriptor_Image_Info::Descriptor_Image_Info(core::shared_ptr<Sampler>     sampler,
                                             core::shared_ptr<Image_View>  imageView,
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
