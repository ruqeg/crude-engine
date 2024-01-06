#include <graphics/vulkan/descriptor_buffer_info.hpp>
#include <graphics/vulkan/buffer.hpp>

namespace crude_engine
{

Descriptor_Buffer_Info::Descriptor_Buffer_Info(Shared_Ptr<Buffer>  buffer,
                                               VkDeviceSize        offset,
                                               VkDeviceSize        range)
  :
  m_buffer(buffer)
{
  this->buffer  = CRUDE_OBJECT_HANDLE(buffer);
  this->offset  = offset;
  this->range   = range;
}

}
