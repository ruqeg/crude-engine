#include <vulkan/vulkan.hpp>

module crude.graphics.descriptor_buffer_info;

namespace crude::graphics
{

Descriptor_Buffer_Info::Descriptor_Buffer_Info(core::Shared_Ptr<Buffer>  buffer,
                                               VkDeviceSize              range,
                                               VkDeviceSize              offset)
  :
  m_buffer(buffer)
{
  this->buffer  = buffer->getHandle();
  this->offset  = offset;
  this->range   = range;
}

}
