module;

#include <vulkan/vulkan.h>

export module crude.gfx.vk.acceleration_structure_input_buffer;

export import crude.gfx.vk.buffer;

export namespace crude::gfx::vk
{

class Acceleration_Structure_Input_Buffer : public Buffer
{
public:
  template<class T>
  explicit Acceleration_Structure_Input_Buffer(core::shared_ptr<Command_Buffer> commandBuffer, const core::vector<T>& data)
    : Acceleration_Structure_Input_Buffer(commandBuffer, core::span<const T>(data)) {}
  template<class T>
  explicit Acceleration_Structure_Input_Buffer(core::shared_ptr<Command_Buffer> commandBuffer, core::span<const T> data)
    : Buffer({
      .device = commandBuffer->getDevice(),
      .size = data.size_bytes(),
      .usage = VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT_KHR | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      .memoryFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
      })
  {
    m_stide = sizeof(T);
    stagedUpload(commandBuffer, data);
  }
  explicit Acceleration_Structure_Input_Buffer(core::shared_ptr<Device> device, VkDeviceSize size)
    : Buffer({
      .device = device,
      .size = size,
      .usage = VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT_KHR | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      .memoryFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
      })
  {}
private:
  core::uint32 m_stide;
};

}
