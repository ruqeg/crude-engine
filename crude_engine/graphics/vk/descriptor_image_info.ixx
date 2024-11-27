module;

#include <vulkan/vulkan.hpp>

export module crude.gfx.vk.descriptor_image_info;

import crude.gfx.vk.sampler;
import crude.gfx.vk.image_view;
import crude.core.std_containers_heap;

export namespace crude::gfx::vk
{

// !TODO

class Descriptor_Image_Info : private VkDescriptorImageInfo
{
public:
  explicit Descriptor_Image_Info(core::shared_ptr<Sampler>     sampler,
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
private:
  core::shared_ptr<Sampler>     m_sampler;
  core::shared_ptr<Image_View>  m_imageView;

  friend class Write_Descriptor_Set;
};

}
