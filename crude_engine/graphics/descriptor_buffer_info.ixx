module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.descriptor_buffer_info;

import crude.core.shared_ptr;
import crude.graphics.buffer;

export namespace crude::graphics
{

struct Descriptor_Buffer_Info : private VkDescriptorBufferInfo
{
public:
  explicit Descriptor_Buffer_Info(core::Shared_Ptr<Buffer>  buffer,
                                  VkDeviceSize              range,
                                  VkDeviceSize              offset = 0);
private:
  core::Shared_Ptr<Buffer>  m_buffer;

  friend class Write_Descriptor_Set;
};

}
