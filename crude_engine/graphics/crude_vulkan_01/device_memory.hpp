#pragma once

#include "../../core/core.hpp"
#include "include_vulkan.hpp"
#include "object.hpp"
#include <optional>

namespace crude_vulkan_01
{

class Device;
class Image;
class Buffer;

struct Device_Memory_Allocate_Info
{
  std::shared_ptr<const Device>  device;
  VkDeviceSize                   allocationSize;
  uint32                         memoryTypeIndex;
  explicit Device_Memory_Allocate_Info(std::shared_ptr<const Device>  device,
                                       VkDeviceSize                   allocationSize,
                                       uint32                         memoryTypeIndex);
  explicit Device_Memory_Allocate_Info(std::shared_ptr<const Device>  device,
                                       VkDeviceSize                   allocationSize,
                                       uint32                         memoryTypeFilter,
                                       VkMemoryPropertyFlags          memoryProperties);
};

class Device_Memory : public TObject<VkDeviceMemory>
{
public:
  explicit Device_Memory(const Device_Memory_Allocate_Info& createInfo);
  ~Device_Memory();
  void bind(Image& image, VkDeviceSize offset = 0u);
  void bind(Buffer& buffer, VkDeviceSize offset = 0u);
  std::optional<void*> map(VkDeviceSize offset = 0u, VkDeviceSize size = VK_WHOLE_SIZE, VkMemoryMapFlags flags = 0);
  void unmap();
private:
  std::shared_ptr<const Device>  m_device;
};

}
