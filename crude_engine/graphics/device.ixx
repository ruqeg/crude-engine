module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.device;

export import crude.core.shared_ptr;
export import crude.core.std_containers_stack;
export import crude.graphics.vulkan_object;
export import crude.graphics.write_descriptor_set;

export namespace crude::graphics
{

class Queue;
class Fence;
class Physical_Device;

struct Device_Queue_Create_Info final : public VkDeviceQueueCreateInfo
{
public:
  explicit Device_Queue_Create_Info(core::uint32                      queueFamilyIndex,
                                    const core::span<core::float32>&  queuePriorities);
};

class Device : public Vulkan_Object<VkDevice>
{
public:
  explicit Device(core::Shared_Ptr<const Physical_Device>      physicalDevice,
                  const core::span<Device_Queue_Create_Info>&  queueDescriptors,
                  const VkPhysicalDeviceFeatures&              enabledFeatures,
                  core::span<const char* const>                enabledExtensions,
                  core::span<const char* const>                enabledLayers);

  ~Device();
  core::Shared_Ptr<const Physical_Device> getPhysicalDevice() const;
  core::Shared_Ptr<Queue> getQueue(core::uint32 queueFamilyIndex, core::uint32 queueIndex) const;
  // !TODO VkCopyDescriptorSet or replace Write_Descriptor_Set back to VkWriteDescriptorSet
  void updateDescriptorSets(const core::span<const Write_Descriptor_Set>&  descriptorWrites,
                            const core::span<const VkCopyDescriptorSet>&   descriptorCopies);
  void waitIdle();
  bool waitForFences(core::span<Fence> fences, bool waitAll, core::uint64 timeout = UINT64_MAX) const;
  bool resetForFences(core::span<Fence> fences) const;
private:
  core::Shared_Ptr<const Physical_Device> m_physicalDevice;
};

}
