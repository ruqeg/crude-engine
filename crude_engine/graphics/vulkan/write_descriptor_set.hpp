#pragma once

#include "descriptor_buffer_info.hpp"
#include "descriptor_image_info.hpp"
#include <optional>

namespace crude_engine
{

class Descriptor_Set;

struct Write_Descriptor_Set : private VkWriteDescriptorSet
{
public:
  explicit Write_Descriptor_Set(std::shared_ptr<Descriptor_Set>               dscriptorSet,
                                uint32                                        binding,
                                uint32                                        arrayElement,
                                uint32                                        descriptorCount,
                                VkDescriptorType                              descriptorType,
                                const std::optional<Descriptor_Image_Info>&   imageInfo,
                                const std::optional<Descriptor_Buffer_Info>&  bufferInfo);
private:
  std::shared_ptr<Descriptor_Set>        m_descriptorSet;
  std::optional<Descriptor_Image_Info>   m_imageInfo;
  std::optional<Descriptor_Buffer_Info>  m_bufferInfo;
  // !TODO const VkBufferView* pTexelBufferView;

  friend class Device;
};

}
