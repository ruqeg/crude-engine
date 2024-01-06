#pragma once

#include <core/data_structures/shared_ptr.hpp>
#include <graphics/vulkan/include_vulkan.hpp>

namespace crude_engine
{

class Buffer;

struct Descriptor_Buffer_Info : private VkDescriptorBufferInfo
{
public:
  explicit Descriptor_Buffer_Info(Shared_Ptr<Buffer>  buffer,
                                  VkDeviceSize        offset,
                                  VkDeviceSize        range);
private:
  Shared_Ptr<Buffer>  m_buffer;

  friend class Write_Descriptor_Set;
};

}
