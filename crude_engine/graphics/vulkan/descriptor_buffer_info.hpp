#pragma once

#include "../../core/core.hpp"
#include "include_vulkan.hpp"

namespace crude_engine
{

class Buffer;

struct Descriptor_Buffer_Info : private VkDescriptorBufferInfo
{
public:
  explicit Descriptor_Buffer_Info(std::shared_ptr<Buffer>  buffer,
                                  VkDeviceSize             offset,
                                  VkDeviceSize             range);
private:
  std::shared_ptr<Buffer>  m_buffer;

  friend class Write_Descriptor_Set;
};

}
