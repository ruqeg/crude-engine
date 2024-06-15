module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.device_queue_create_info;

export import crude.core.alias;
export import crude.core.std_containers_stack;
export import crude.core.std_containers_heap;

export namespace crude::graphics
{

class Surface;
class Physical_Device;

class Device_Queue_Create_Info final : public VkDeviceQueueCreateInfo
{
public:
  explicit Device_Queue_Create_Info(core::shared_ptr<const Physical_Device>  physicalDevice,
                                    VkQueueFlagBits                          queueType,
                                    const core::span<core::float32>&         queuePriorities);
  explicit Device_Queue_Create_Info(core::shared_ptr<const Physical_Device>  physicalDevice,
                                    core::shared_ptr<const Surface>          surface,
                                    const core::span<core::float32>&         queuePriorities);
private:
  core::uint32 chooseFamilyIndex(VkQueueFlagBits queueType, const core::vector<VkQueueFamilyProperties>& queueFamilyProperties) const;
  core::uint32 chooseFamilyIndex(core::shared_ptr<const Physical_Device>       physicalDevice,
                                 core::shared_ptr<const Surface>               surface, 
                                 const core::vector<VkQueueFamilyProperties>&  queueFamilyProperties) const;
};

}