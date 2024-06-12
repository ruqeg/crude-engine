module;

#include <vulkan/vulkan.h>

export module crude.graphics.vertex_buffer;

export import crude.graphics.buffer;

export namespace crude::graphics
{

class Vertex_Buffer : public Buffer
{
public:
  template<class T>
  explicit Vertex_Buffer(core::shared_ptr<Command_Buffer> commandBuffer, core::span<const T> data)
    :
    Buffer(commandBuffer->getDevice(), data.size_bytes(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
  {
    stagedUpload(commandBuffer, data);
  }
};

}
