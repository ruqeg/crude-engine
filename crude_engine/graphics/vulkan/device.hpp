#pragma once

#include <graphics/vulkan/object.hpp>
#include <graphics/vulkan/write_descriptor_set.hpp>

namespace crude_engine
{

class Queue;
class Fence;
class Physical_Device;

struct Device_Queue_Create_Info final : public VkDeviceQueueCreateInfo
{
public:
  explicit Device_Queue_Create_Info(uint32          queueFamilyIndex,
                                    const float32*  pQueuePriorities,
                                    uint32          queuePrioritiCount);
};

class Device : public TObject<VkDevice>
{
public:
  explicit Device(Shared_Ptr<const Physical_Device>  physicalDevice,
                  Device_Queue_Create_Info*          pQueueDescriptors,
                  uint32                             queueDescriptorCount,
                  const VkPhysicalDeviceFeatures*    pEnabledFeatures,
                  uint32                             enabledFeatureCount,
                  const char**                       pEnabledExtensions,
                  uint32                             enabledExtensionCount,
                  const char**                       pEnabledLayers,
                  uint32                             enabledLayerCount);

  ~Device();
  Shared_Ptr<const Physical_Device> getPhysicalDevice() const;
  Shared_Ptr<Queue> getQueue(uint32 queueFamilyIndex, uint32 queueIndex) const;
  // !TODO VkCopyDescriptorSet or replace Write_Descriptor_Set back to VkWriteDescriptorSet
  void updateDescriptorSets(const Write_Descriptor_Set*  pDescriptorWrites, 
                            const uint32                 descriptorWriteCount,
                            const VkCopyDescriptorSet*   pDescriptorCopies,
                            const uint32                 descriptorCopieCount);
  void waitIdle();
  bool waitForFences(Fence* pFences, uint32 fenceCount, bool waitAll, uint64 timeout = std::numeric_limits<uint64>::max()) const;
  bool resetForFences(Fence* pFences, uint32 fenceCount) const;
private:
  Shared_Ptr<const Physical_Device> m_physicalDevice;
};

}
