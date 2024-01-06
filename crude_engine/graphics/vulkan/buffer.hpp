#pragma once

#include <core/data_structures/shared_ptr.hpp>
#include <graphics/vulkan/include_vulkan.hpp>
#include <graphics/vulkan/object.hpp>

namespace crude_engine
{

class Device;

class Buffer : public TObject<VkBuffer>
{
public:
  explicit Buffer(Shared_Ptr<const Device>  device,
                  VkDeviceSize              size,
                  VkBufferUsageFlags        usage,
                  VkSharingMode             sharingMode,
                  uint32*                   pQueueFamilyIndices,
                  uint32                    queueFamilyIndexCount);
  ~Buffer();
  VkMemoryRequirements getMemoryRequirements() const;
private:
  const Shared_Ptr<const Device>  m_device;
};

}
