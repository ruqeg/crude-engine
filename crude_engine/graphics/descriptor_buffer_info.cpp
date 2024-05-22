#include <vulkan/vulkan.hpp>

module crude.graphics.vulkan.descriptor_buffer_info;

namespace crude::graphics
{

Descriptor_Buffer_Info::Descriptor_Buffer_Info(core::Shared_Ptr<Buffer>  buffer,
                                               VkDeviceSize              offset,
                                               VkDeviceSize              range)
  :
  m_buffer(buffer)
{
  this->buffer  = buffer->getHandle();
  this->offset  = offset;
  this->range   = range;
}

}
