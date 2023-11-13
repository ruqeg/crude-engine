#pragma once

#include "core.hpp"
#include "include_vulkan.hpp"
#include "object.hpp"

namespace crude_vulkan_01
{

class Device;
class Image;

struct Device_Memory_Allocate_Info
{
  std::shared_ptr<const Device>  device;
  VkDeviceSize                   allocationSize;
  uint32                         memoryTypeIndex;
  explicit Device_Memory_Allocate_Info(std::shared_ptr<const Device>  device,
                                       VkDeviceSize                   allocationSize,
                                       uint32                         memoryTypeIndex);
};

class Device_Memory : public TObject<VkDeviceMemory>
{
public:
  explicit Device_Memory(const Device_Memory_Allocate_Info& createInfo);
  void bind(Image& image, VkDeviceSize offset);
  ~Device_Memory();
private:
  std::shared_ptr<const Device>  m_device;
};

}
