module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.descriptor_buffer_info;

import crude.core.std_containers_heap;
import crude.graphics.buffer;

export namespace crude::graphics
{

struct Descriptor_Buffer_Info : private VkDescriptorBufferInfo
{
public:
  explicit Descriptor_Buffer_Info(core::shared_ptr<Buffer>  buffer,
                                  VkDeviceSize              range,
                                  VkDeviceSize              offset = 0);
private:
  core::shared_ptr<Buffer>  m_buffer;

  friend class Write_Descriptor_Set;
};

}
