module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.descriptor_image_info;

import crude.graphics.sampler;
import crude.graphics.image_view;
import crude.core.shared_ptr;

export namespace crude::graphics
{

class Descriptor_Image_Info : private VkDescriptorImageInfo
{
public:
  explicit Descriptor_Image_Info(core::Shared_Ptr<Sampler>     sampler,
                                 core::Shared_Ptr<Image_View>  imageView,
                                 VkImageLayout                 imageLayout);
private:
  core::Shared_Ptr<Sampler>     m_sampler;
  core::Shared_Ptr<Image_View>  m_imageView;

  friend class Write_Descriptor_Set;
};

}
