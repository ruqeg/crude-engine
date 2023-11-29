#pragma once

#include "../../core/core.hpp"
#include "include_vulkan.hpp"
#include "object.hpp"
#include <vector>

namespace crude_vulkan_01
{

class Device;

struct Buffer_Create_Info
{
  std::shared_ptr<const Device>  device;
  VkDeviceSize                   size;
  VkBufferUsageFlags             usage;
  VkSharingMode                  sharingMode;
  std::vector<uint32>            queueFamilyIndices;
  explicit Buffer_Create_Info(std::shared_ptr<const Device>  device,
                              VkDeviceSize                   size,
                              VkBufferUsageFlags             usage,
                              VkSharingMode                  sharingMode,
                              const std::vector<uint32>&     queueFamilyIndices);
};

class Buffer : public TObject<VkBuffer>
{
public:
  explicit Buffer(const Buffer_Create_Info& createInfo);
  ~Buffer();
  VkMemoryRequirements getMemoryRequirements() const;
private:
  std::shared_ptr<const Device>  m_device;
};

}
