module;

#include <vulkan/vulkan.h>

export module crude.graphics.buffer;

import crude.core.shared_ptr;
import crude.core.std_containers_stack;
import crude.graphics.vulkan_object;

export namespace crude::graphics
{

class Device;

class Buffer : public Vulkan_Object<VkBuffer>
{
public:
  explicit Buffer(core::Shared_Ptr<const Device>  device,
                  VkDeviceSize                    size,
                  VkBufferUsageFlags              usage);
  explicit Buffer(core::Shared_Ptr<const Device>  device,
                  VkDeviceSize                    size,
                  VkBufferUsageFlags              usage,
                  core::span<core::uint32>        queueFamilyIndices);
  ~Buffer();
  VkMemoryRequirements getMemoryRequirements() const;
private:
  explicit Buffer(core::Shared_Ptr<const Device>  device,
                  VkDeviceSize                    size,
                  VkBufferUsageFlags              usage,
                  VkSharingMode                   sharingMode,
                  core::span<core::uint32>        queueFamilyIndices);
private:
  const core::Shared_Ptr<const Device>  m_device;
  VkDeviceSize                          m_size;
};

}
