module;

#include <vulkan/vulkan.h>

export module crude_engine.graphics.vulkan.buffer;

import crude_engine.core.shared_ptr;
import crude_engine.core.std_containers_stack;
import crude_engine.graphics.vulkan.vulkan_object;

export namespace crude_engine
{

class Device;

class Buffer : public Vulkan_Object<VkBuffer>
{
public:
  explicit Buffer(Shared_Ptr<const Device>  device,
                  VkDeviceSize              size,
                  VkBufferUsageFlags        usage,
                  VkSharingMode             sharingMode,
                  span<uint32>              queueFamilyIndices);
  ~Buffer();
  VkMemoryRequirements getMemoryRequirements() const;
private:
  const Shared_Ptr<const Device>  m_device;
};

}
