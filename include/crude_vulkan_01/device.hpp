#pragma once

#include "core.hpp"
#include "object.hpp"
#include "include_vulkan.hpp"
#include "device_queue_create_info.hpp"

namespace crude_vulkan_01
{

class Physical_Device;

struct DeviceCreateInfo
{
  std::shared_ptr<const Physical_Device> physicalDevice;
  VkPhysicalDeviceFeatures               enabledFeatures;
  std::vector<const char*>               enabledExtensions;
  std::vector<const char*>               enabledLayers;
  std::vector<DeviceQueueCreateInfo>     queueDescriptors;

  explicit DeviceCreateInfo(std::shared_ptr<const Physical_Device>     physicalDevice,
                            const std::vector<DeviceQueueCreateInfo>&  queueDescriptors,
                            const VkPhysicalDeviceFeatures&            enabledFeatures = {},
                            const std::vector<const char*>&            enabledExtensions = {},
                            const std::vector<const char*>&            enabledLayers = {});
};

class Device : TObject<VkDevice>
{
public:
  explicit Device(const DeviceCreateInfo& createinfo);
  ~Device();
  std::shared_ptr<const Physical_Device> getPhysicalDevice() const;
private:
  std::shared_ptr<const Physical_Device> m_physicalDevice;
};

}
