module;

#include <vulkan/vulkan.h>

export module crude.gfx.vk.acceleration_structure_storage_buffer;

export import crude.gfx.vk.buffer;

export namespace crude::gfx::vk
{

class Acceleration_Structure_Storage_Buffer : public Buffer
{
public:
  explicit Acceleration_Structure_Storage_Buffer(core::shared_ptr<Device> device, VkDeviceSize size)
    : Buffer{ vk::Buffer::Initialize{
      .device = device,
      .size = size,
      .usage = VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
      .memoryFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT } }
  {}
};

}
