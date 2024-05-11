module;

#include <vulkan/vulkan.hpp>

export module crude_engine.graphics.vulkan.descriptor_image_info;

import crude_engine.graphics.vulkan.sampler;
import crude_engine.graphics.vulkan.image_view;
import crude_engine.core.shared_ptr;

export namespace crude_engine
{

class Descriptor_Image_Info : private VkDescriptorImageInfo
{
public:
  explicit Descriptor_Image_Info(Shared_Ptr<Sampler>     sampler,
                                 Shared_Ptr<Image_View>  imageView,
                                 VkImageLayout           imageLayout);
private:
  Shared_Ptr<Sampler>     m_sampler;
  Shared_Ptr<Image_View>  m_imageView;

  friend class Write_Descriptor_Set;
};

}
