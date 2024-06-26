module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.device_queue_descriptor;

export import crude.core.alias;
export import crude.core.std_containers_stack;
export import crude.core.std_containers_heap;

export namespace crude::graphics
{

class Surface;
class Physical_Device;

enum Device_Queue_Denotation_Flag_Bits
{
  DEVICE_QUEUE_DENOTATION_GRAPHICS_BITS = 0x00000000,
  DEVICE_QUEUE_DENOTATION_TRANSFER_BITS = 0x0000000F,
  DEVICE_QUEUE_DENOTATION_PRESENT_BITS  = 0x000000F0,
  DEVICE_QUEUE_DENOTATION_MAX_BITS      = 0xF0000000,
};

Device_Queue_Denotation_Flag_Bits queueFlagBitsToDenotation(VkQueueFlagBits flag);

class Device_Queue_Descriptor final : public VkDeviceQueueCreateInfo
{
public:
  explicit Device_Queue_Descriptor(core::shared_ptr<const Physical_Device>  physicalDevice,
                                   VkQueueFlagBits                          queueType,
                                   core::span<const core::float32>          queuePriorities = core::array<core::float32, 1>{1.f});
  explicit Device_Queue_Descriptor(core::shared_ptr<const Physical_Device>  physicalDevice,
                                   core::shared_ptr<const Surface>          surface,
                                   core::span<const core::float32>          queuePriorities = core::array<core::float32, 1>{1.f});
  bool operator==(const Device_Queue_Descriptor& other) const;
  bool operator!=(const Device_Queue_Descriptor& other) const;
  Device_Queue_Denotation_Flag_Bits getDenotation() const { return m_queueDenotation; }
private:
  core::uint32 chooseFamilyIndex(VkQueueFlagBits queueType, const core::vector<VkQueueFamilyProperties>& queueFamilyProperties) const;
  core::uint32 chooseFamilyIndex(core::shared_ptr<const Physical_Device>       physicalDevice,
                                 core::shared_ptr<const Surface>               surface, 
                                 const core::vector<VkQueueFamilyProperties>&  queueFamilyProperties) const;
private:
  Device_Queue_Denotation_Flag_Bits  m_queueDenotation;
};

}