#include <vulkan/vulkan.hpp>

module crude_engine.graphics.vulkan.descriptor_buffer_info;

import crude_engine.graphics.vulkan.buffer;

namespace crude_engine
{

Descriptor_Buffer_Info::Descriptor_Buffer_Info(Shared_Ptr<Buffer>  buffer,
                                               VkDeviceSize        offset,
                                               VkDeviceSize        range)
  :
  m_buffer(buffer)
{
  this->buffer  = buffer->getHandle();
  this->offset  = offset;
  this->range   = range;
}

}
