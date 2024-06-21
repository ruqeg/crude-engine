module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.descriptor_pool_size;

import crude.core.alias;

export namespace crude::graphics
{

class Descriptor_Pool_Size : public VkDescriptorPoolSize
{
public:
  Descriptor_Pool_Size(VkDescriptorType type, core::uint32 descriptorCount)
  {
    this->type = type;
    this->descriptorCount = descriptorCount;
  }
};

class Uniform_Buffer_Pool_Size : public Descriptor_Pool_Size
{
public:
  Uniform_Buffer_Pool_Size(core::uint32 descriptorCount)
    : Descriptor_Pool_Size(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, descriptorCount) 
  {}
};

class Combined_Image_Sampler_Pool_Size : public Descriptor_Pool_Size
{
public:
  Combined_Image_Sampler_Pool_Size(core::uint32 descriptorCount)
    : Descriptor_Pool_Size(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, descriptorCount)
  {}
};

}