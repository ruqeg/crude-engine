#pragma once

#include "../../core/core.hpp"
#include "include_vulkan.hpp"

namespace crude_engine
{

class Sampler;
class Image_View;

struct Descriptor_Image_Info : private VkDescriptorImageInfo
{
public:
  explicit Descriptor_Image_Info(std::shared_ptr<Sampler>     sampler,
                                 std::shared_ptr<Image_View>  imageView,
                                 VkImageLayout                imageLayout);
private:
  std::shared_ptr<Sampler>     m_sampler;
  std::shared_ptr<Image_View>  m_imageView;

  friend class Write_Descriptor_Set;
};

}
