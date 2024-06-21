module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.write_descriptor_set;

export import crude.graphics.buffer_descriptor;
export import crude.graphics.image_descriptor;

export namespace crude::graphics
{

class Descriptor_Set;

class Write_Descriptor_Set : private VkWriteDescriptorSet
{
public:
  explicit Write_Descriptor_Set(core::shared_ptr<Descriptor_Set>  descriptorSet,
                                Uniform_Buffer_Descriptor         uniformbufferDesc);
  explicit Write_Descriptor_Set(core::shared_ptr<Descriptor_Set>   descriptorSet,
                                Combined_Image_Sampler_Descriptor  samplerDesc);

private:
  core::shared_ptr<Descriptor_Set>  m_descriptorSet;
  union
  {
  VkDescriptorBufferInfo            m_bufferInfo;
  VkDescriptorImageInfo             m_imageInfo;
  };

  friend class Device;
};

}
