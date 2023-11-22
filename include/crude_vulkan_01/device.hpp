#pragma once

#include "object.hpp"
#include "write_descriptor_set.hpp"
#include <vector>

namespace crude_vulkan_01
{

class Queue;
class Physical_Device;

struct Device_Queue_Create_Info final : public VkDeviceQueueCreateInfo
{
public:
  explicit Device_Queue_Create_Info(uint32                       queueFamilyIndex,
                                    const std::vector<float32>&  queuePriorities = {1.f});
};

struct Device_Create_Info
{
  std::shared_ptr<const Physical_Device>  physicalDevice;
  VkPhysicalDeviceFeatures                enabledFeatures;
  std::vector<const char*>                enabledExtensions;
  std::vector<const char*>                enabledLayers;
  std::vector<Device_Queue_Create_Info>   queueDescriptors;

  explicit Device_Create_Info(std::shared_ptr<const Physical_Device>        physicalDevice,
                              const std::vector<Device_Queue_Create_Info>&  queueDescriptors,
                              const VkPhysicalDeviceFeatures&               enabledFeatures = {},
                              const std::vector<const char*>&               enabledExtensions = {},
                              const std::vector<const char*>&               enabledLayers = {});
};

class Device : public TObject<VkDevice>
{
public:
  explicit Device(const Device_Create_Info& createinfo);
  ~Device();
  std::shared_ptr<const Physical_Device> getPhysicalDevice() const;
  std::shared_ptr<Queue> getQueue(uint32 queueFamilyIndex, uint32 queueIndex) const;
  // !TODO VkCopyDescriptorSet or replace Write_Descriptor_Set back to VkWriteDescriptorSet
  void updateDescriptorSets(const std::vector<Write_Descriptor_Set>& descriptorWrites, const std::vector<VkCopyDescriptorSet>& descriptorCopies);
  void waitIdle();
private:
  std::shared_ptr<const Physical_Device> m_physicalDevice;
};

}
