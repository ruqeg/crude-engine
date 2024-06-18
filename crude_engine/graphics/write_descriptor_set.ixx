module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.write_descriptor_set;

export import crude.graphics.buffer_descriptor;

export namespace crude::graphics
{

class Descriptor_Set;

class Write_Descriptor_Set : private VkWriteDescriptorSet
{
public:
  explicit Write_Descriptor_Set(core::shared_ptr<Descriptor_Set>  descriptorSet,
                                Uniform_Buffer_Descriptor         uniformbufferDesc);

private:
  core::shared_ptr<Descriptor_Set>  m_descriptorSet;
  VkDescriptorBufferInfo            m_bufferInfo;

  friend class Device;
};

}
