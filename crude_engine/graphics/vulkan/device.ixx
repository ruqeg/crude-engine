module;

#include <vulkan/vulkan.hpp>

export module crude_engine.graphics.vulkan.device;

import crude_engine.core.shared_ptr;
import crude_engine.core.std_containers_stack;
import crude_engine.graphics.vulkan.vulkan_object;
import crude_engine.graphics.vulkan.write_descriptor_set;

export namespace crude_engine
{

class Queue;
class Fence;
class Physical_Device;

struct Device_Queue_Create_Info final : public VkDeviceQueueCreateInfo
{
public:
  explicit Device_Queue_Create_Info(uint32                queueFamilyIndex,
                                    const span<float32>&  queuePriorities);
};

class Device : public Vulkan_Object<VkDevice>
{
public:
  explicit Device(Shared_Ptr<const Physical_Device>      physicalDevice,
                  const span<Device_Queue_Create_Info>&  queueDescriptors,
                  const VkPhysicalDeviceFeatures&        enabledFeatures,
                  span<const char*>                      enabledExtensions,
                  span<const char*>                      enabledLayers);

  ~Device();
  Shared_Ptr<const Physical_Device> getPhysicalDevice() const;
  Shared_Ptr<Queue> getQueue(uint32 queueFamilyIndex, uint32 queueIndex) const;
  // !TODO VkCopyDescriptorSet or replace Write_Descriptor_Set back to VkWriteDescriptorSet
  void updateDescriptorSets(const span<Write_Descriptor_Set>&  descriptorWrites,
                            const span<VkCopyDescriptorSet>&   descriptorCopies);
  void waitIdle();
  bool waitForFences(span<Fence> fences, bool waitAll, uint64 timeout = UINT64_MAX) const;
  bool resetForFences(span<Fence> fences) const;
private:
  Shared_Ptr<const Physical_Device> m_physicalDevice;
};

}
