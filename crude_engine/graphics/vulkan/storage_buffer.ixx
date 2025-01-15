module;

#include <vulkan/vulkan.h>

export module crude.gfx.vk.storage_buffer;

export import crude.gfx.vk.buffer;
import crude.gfx.vk.command_buffer;

export namespace crude::gfx::vk
{

class Storage_Buffer : public Buffer
{
public:
  template<class T>
  explicit Storage_Buffer(core::shared_ptr<Command_Buffer> commandBuffer, const core::vector<T>& data)
    : Storage_Buffer(commandBuffer, data.size(), data.data()) {}
  template<class T, core::size_t N>
  explicit Storage_Buffer(core::shared_ptr<Command_Buffer> commandBuffer, const core::array<T, N>& data)
    : Storage_Buffer(commandBuffer, data.size(), data.data()) {}
  template<class T>
  explicit Storage_Buffer(core::shared_ptr<Command_Buffer> commandBuffer, core::span<const T> data)
    : Storage_Buffer(commandBuffer, data.size(), data.data()) {}
  explicit Storage_Buffer(core::shared_ptr<Command_Buffer> commandBuffer, VkDeviceSize size, const void* data = nullptr)
    : Buffer({
      .device = commandBuffer->getDevice(),
      .size = size,
      .usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
      .memoryFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
      })
  {
    if (data)
    {
      stagedUpload(commandBuffer, data, size);
    }
  }
};

}
