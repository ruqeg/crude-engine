module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.write_descriptor_set;

export import crude.graphics.buffer_descriptor;
export import crude.graphics.image_descriptor;

export namespace crude::graphics
{

class Descriptor_Set;

class Write_Descriptor_Set : public VkWriteDescriptorSet
{
protected:
  explicit Write_Descriptor_Set(core::shared_ptr<Descriptor_Set> descriptorSet);
private:
  core::shared_ptr<Descriptor_Set>  m_descriptorSet;
};

class Buffer_Write_Descriptor_Set : public Write_Descriptor_Set
{
public:
  explicit Buffer_Write_Descriptor_Set(core::shared_ptr<Descriptor_Set> descriptorSet, const Buffer_Descriptor& bufferDescriptor);
private:
  VkDescriptorBufferInfo  m_bufferInfo;
};

class Image_Write_Descriptor_Set : public Write_Descriptor_Set
{
public:
  explicit Image_Write_Descriptor_Set(core::shared_ptr<Descriptor_Set> descriptorSet, const Image_Descriptor& imageDescriptor);
private:
  VkDescriptorImageInfo m_imageInfo;
};

}
