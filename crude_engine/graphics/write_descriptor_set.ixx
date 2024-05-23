module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.write_descriptor_set;

import crude.graphics.descriptor_buffer_info;
import crude.graphics.descriptor_image_info;
import crude.core.optional;
import crude.core.shared_ptr;

export namespace crude::graphics
{

class Descriptor_Set;

class Write_Descriptor_Set : private VkWriteDescriptorSet
{
public:
  explicit Write_Descriptor_Set(core::Shared_Ptr<Descriptor_Set>               descriptorSet,
                                core::uint32                                   binding,
                                core::uint32                                   arrayElement,
                                core::uint32                                   descriptorCount,
                                VkDescriptorType                               descriptorType,
                                const core::Optional<Descriptor_Image_Info>&   imageInfo,
                                const core::Optional<Descriptor_Buffer_Info>&  bufferInfo);
private:
  core::Shared_Ptr<Descriptor_Set>        m_descriptorSet;
  core::Optional<Descriptor_Image_Info>   m_imageInfo;
  core::Optional<Descriptor_Buffer_Info>  m_bufferInfo;
  // !TODO const VkBufferView* pTexelBufferView;

  friend class Device;
};

}
