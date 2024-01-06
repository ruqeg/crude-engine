#include <graphics/vulkan/descriptor_image_info.hpp>
#include <graphics/vulkan/sampler.hpp>
#include <graphics/vulkan/image_view.hpp>


namespace crude_engine
{

Descriptor_Image_Info::Descriptor_Image_Info(Shared_Ptr<Sampler>     sampler,
                                             Shared_Ptr<Image_View>  imageView,
                                             VkImageLayout                imageLayout)
  :
  m_sampler(sampler),
  m_imageView(imageView)
{
  this->sampler      = CRUDE_OBJECT_HANDLE(m_sampler);
  this->imageView    = CRUDE_OBJECT_HANDLE(m_imageView);
  this->imageLayout  = imageLayout;
}

}
