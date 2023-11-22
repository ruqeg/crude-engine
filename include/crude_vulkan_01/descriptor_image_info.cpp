#include "descriptor_image_info.hpp"
#include "sampler.hpp"
#include "image_view.hpp"


namespace crude_vulkan_01
{

Descriptor_Image_Info::Descriptor_Image_Info(std::shared_ptr<Sampler>     sampler,
                                             std::shared_ptr<Image_View>  imageView,
                                             VkImageLayout                imageLayout)
  :
  m_sampler(sampler),
  m_imageView(imageView)
{
  this->sampler      = CRUDE_VULKAN_01_HANDLE(m_sampler);
  this->imageView    = CRUDE_VULKAN_01_HANDLE(m_imageView);
  this->imageLayout  = imageLayout;
}

}
