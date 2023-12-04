#include "descriptor_buffer_info.hpp"
#include "buffer.hpp"

namespace crude_engine
{

Descriptor_Buffer_Info::Descriptor_Buffer_Info(std::shared_ptr<Buffer>  buffer,
                                               VkDeviceSize             offset,
                                               VkDeviceSize             range)
  :
  m_buffer(buffer)
{
  this->buffer  = CRUDE_VULKAN_01_HANDLE(buffer);
  this->offset  = offset;
  this->range   = range;
}

}
