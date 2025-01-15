module;

#include <vulkan/vulkan.h>

export module crude.gfx.vk.vertex_buffer;

export import crude.gfx.vk.buffer;
import crude.gfx.vk.command_buffer;

export namespace crude::gfx::vk
{

class Vertex_Buffer : public Buffer
{
public:
  template<class T>
  explicit Vertex_Buffer(core::shared_ptr<Command_Buffer> commandBuffer, const core::vector<T>& data)
    : Vertex_Buffer(commandBuffer, core::span<const T>(data)) {}
  template<class T>
  explicit Vertex_Buffer(core::shared_ptr<Command_Buffer> commandBuffer, core::span<const T> data)
    : Buffer({
      .device = commandBuffer->getDevice(),
      .size = data.size_bytes(),
      .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      .memoryFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
      })
  {
    stagedUpload(commandBuffer, data);
  }
};

}
