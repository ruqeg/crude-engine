#pragma once

#include "core.hpp"
#include "include_vulkan.hpp"

namespace crude_vulkan_01
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
