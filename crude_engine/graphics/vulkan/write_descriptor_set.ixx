module;

#include <vulkan/vulkan.hpp>

export module crude_engine.graphics.vulkan.write_descriptor_set;

import crude_engine.graphics.vulkan.descriptor_buffer_info;
import crude_engine.graphics.vulkan.descriptor_image_info;
import crude_engine.core.optional;
import crude_engine.core.shared_ptr;

export namespace crude_engine
{

class Descriptor_Set;

class Write_Descriptor_Set : private VkWriteDescriptorSet
{
public:
  explicit Write_Descriptor_Set(Shared_Ptr<Descriptor_Set>               descriptorSet,
                                uint32                                   binding,
                                uint32                                   arrayElement,
                                uint32                                   descriptorCount,
                                VkDescriptorType                         descriptorType,
                                const Optional<Descriptor_Image_Info>&   imageInfo,
                                const Optional<Descriptor_Buffer_Info>&  bufferInfo);
private:
  Shared_Ptr<Descriptor_Set>        m_descriptorSet;
  Optional<Descriptor_Image_Info>   m_imageInfo;
  Optional<Descriptor_Buffer_Info>  m_bufferInfo;
  // !TODO const VkBufferView* pTexelBufferView;

  friend class Device;
};

}
