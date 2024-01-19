#pragma once

#include <graphics/vulkan/sampler.hpp>
#include <graphics/vulkan/image_view.hpp>

namespace crude_engine
{

class Sampler;
class Image_View;

struct Descriptor_Image_Info : private VkDescriptorImageInfo
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
