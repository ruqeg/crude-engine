#pragma once

#include <core/data_structures/array_unsafe.hpp>
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
  explicit Device_Queue_Create_Info(uint32                        queueFamilyIndex,
                                    const Array_Unsafe<float32>&  queuePriorities);
};

class Device : public TObject<VkDevice>
{
public:
  explicit Device(Shared_Ptr<const Physical_Device>              physicalDevice,
                  const Array_Unsafe<Device_Queue_Create_Info>&  queueDescriptors,
                  const VkPhysicalDeviceFeatures&                enabledFeatures,
                  Array_Unsafe<const char*>                      enabledExtensions,
                  Array_Unsafe<const char*>                      enabledLayers);

  ~Device();
  Shared_Ptr<const Physical_Device> getPhysicalDevice() const;
  Shared_Ptr<Queue> getQueue(uint32 queueFamilyIndex, uint32 queueIndex) const;
  // !TODO VkCopyDescriptorSet or replace Write_Descriptor_Set back to VkWriteDescriptorSet
  void updateDescriptorSets(const Array_Unsafe<Write_Descriptor_Set>&  descriptorWrites,
                            const Array_Unsafe<VkCopyDescriptorSet>&   descriptorCopies);
  void waitIdle();
  bool waitForFences(Array_Unsafe<Fence> fences, bool waitAll, uint64 timeout = UINT64_MAX) const;
  bool resetForFences(Array_Unsafe<Fence> fences) const;
private:
  Shared_Ptr<const Physical_Device> m_physicalDevice;
};

}
