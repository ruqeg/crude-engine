module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.device;

export import crude.core.optional;
export import crude.core.std_containers_heap;
export import crude.core.std_containers_stack;
export import crude.graphics.vulkan_object;
export import crude.graphics.write_descriptor_set;
export import crude.graphics.device_queue_create_info;

export namespace crude::graphics
{

class Queue;
class Fence;
class Physical_Device;

class Device : public Vulkan_Object<VkDevice>
{
public:
  explicit Device(core::shared_ptr<const Physical_Device>      physicalDevice,
                  const core::span<Device_Queue_Create_Info>&  queueDescriptors,
                  const VkPhysicalDeviceFeatures&              enabledFeatures,
                  core::span<const char* const>                enabledExtensions,
                  core::span<const char* const>                enabledLayers);

  ~Device();
  core::shared_ptr<const Physical_Device> getPhysicalDevice() const;
  core::Optional<core::shared_ptr<Queue>> getQueue(VkQueueFlagBits flags, core::uint32 queueIndex) const;
  core::Optional<core::shared_ptr<Queue>> getQueueByFamily(core::uint32 queueFamilyIndex, core::uint32 queueIndex = 0u) const;
  // !TODO VkCopyDescriptorSet or replace Write_Descriptor_Set back to VkWriteDescriptorSet
  void updateDescriptorSets(const core::span<const Write_Descriptor_Set>&  descriptorWrites,
                            const core::span<const VkCopyDescriptorSet>&   descriptorCopies);
  void waitIdle();
  bool waitForFences(core::span<Fence> fences, bool waitAll, core::uint64 timeout = UINT64_MAX) const;
  bool resetForFences(core::span<Fence> fences) const;
private:
  core::shared_ptr<const Physical_Device>  m_physicalDevice;
};

}
