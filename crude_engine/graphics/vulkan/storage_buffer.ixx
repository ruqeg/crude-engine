module;

#include <vulkan/vulkan.h>

export module crude.gfx.vk.storage_buffer;

export import crude.gfx.vk.buffer;

export namespace crude::gfx::vk
{

class Storage_Buffer : public Buffer
{
public:
  template<class T>
  explicit Storage_Buffer(core::shared_ptr<Command_Buffer> commandBuffer, const core::vector<T>& data)
    : Storage_Buffer(commandBuffer, core::span<const T>(data))
  {}
  template<class T, core::size_t N>
  explicit Storage_Buffer(core::shared_ptr<Command_Buffer> commandBuffer, const core::array<T, N>& data)
    : Storage_Buffer(commandBuffer, core::span<const T>(data))
  {}
  template<class T>
  explicit Storage_Buffer(core::shared_ptr<Command_Buffer> commandBuffer, core::span<const T> data)
    : Buffer({
      .device = commandBuffer->getDevice(),
      .size = data.size_bytes(),
      .usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      .memoryFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
      })
  {
    stagedUpload(commandBuffer, data);
  }
};

}
