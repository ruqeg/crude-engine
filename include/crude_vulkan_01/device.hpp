#pragma once

#include "core.hpp"
#include "object.hpp"
#include "include_vulkan.hpp"
#include <vector>

namespace crude_vulkan_01
{

class Queue;
class Physical_Device;

struct DeviceQueueCreateInfo final : public VkDeviceQueueCreateInfo
{
public:
  explicit DeviceQueueCreateInfo(uint32                       queueFamilyIndex,
                                 const std::vector<float32>&  queuePriorities = {1.f});
};

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

class Device : public TObject<VkDevice>
{
public:
  explicit Device(const DeviceCreateInfo& createinfo);
  ~Device();
  std::shared_ptr<const Physical_Device> getPhysicalDevice() const;
  std::shared_ptr<Queue> getQueue(uint32 queueFamilyIndex, uint32 queueIndex) const;
private:
  std::shared_ptr<const Physical_Device> m_physicalDevice;
};

}
