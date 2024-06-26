module;

#include <algorithm>
#include <vulkan/vulkan.h>

export module crude.graphics.staging_buffer;

export import crude.graphics.buffer;

export namespace crude::graphics
{

class Staging_Buffer : public Buffer
{
public:
  template<class T>
  explicit Staging_Buffer(core::shared_ptr<const Device> device, core::span<const T> data)
    : Staging_Buffer(device, data.data(), data.size_bytes()) {}
  explicit Staging_Buffer(core::shared_ptr<const Device> device, const void* data, VkDeviceSize size)
    : Buffer({
      .device = device, 
      .size = size, 
      .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      .memoryFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT })
  {
    copyHost(data, size);
  }
};

}