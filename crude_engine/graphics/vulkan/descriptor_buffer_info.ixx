module;

#include <vulkan/vulkan.hpp>

export module crude_engine.graphics.vulkan.descriptor_buffer_info;

import crude_engine.core.shared_ptr;
import crude_engine.graphics.vulkan.buffer;

export namespace crude_engine
{

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
