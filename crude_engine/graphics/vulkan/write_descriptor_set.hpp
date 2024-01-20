#pragma once

#include <graphics/vulkan/descriptor_buffer_info.hpp>
#include <graphics/vulkan/descriptor_image_info.hpp>
#include <core/data_structures/optional.hpp>
#include <core/data_structures/shared_ptr.hpp>

namespace crude_engine
{

class Descriptor_Set;

struct Write_Descriptor_Set : private VkWriteDescriptorSet
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
