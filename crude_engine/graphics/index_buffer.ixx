module;

#include <vulkan/vulkan.h>

export module crude.graphics.index_buffer;

export import crude.graphics.buffer;

export namespace crude::graphics
{

class Index_Buffer : public Buffer
{
public:
  template<class T>
  explicit Index_Buffer(core::shared_ptr<Command_Buffer> commandBuffer, VkIndexType indexType, const core::vector<T>& data)
    : Index_Buffer(commandBuffer, indexType, core::span<const T>(data))
  {}
  template<class T>
  explicit Index_Buffer(core::shared_ptr<Command_Buffer> commandBuffer, VkIndexType indexType, core::span<const T> data)
    :
    Buffer(commandBuffer->getDevice(), data.size_bytes(), VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
  {
    m_indexType = indexType;
    stagedUpload(commandBuffer, data);
  }
  VkIndexType getIndexType() const { return m_indexType; }
private:
  VkIndexType m_indexType;
};

}
