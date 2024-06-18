module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.buffer_descriptor;

export import crude.graphics.descriptor_set_layout_binding;
export import crude.core.std_containers_heap;
export import crude.graphics.uniform_buffer;

export namespace crude::graphics
{

class Uniform_Buffer_Descriptor : public Descriptor_Set_Layout_Binding
{
public:
  Uniform_Buffer_Descriptor(core::uint32 binding, VkShaderStageFlags stageFlags);
  template<class T>
  void update(core::shared_ptr<Uniform_Buffer<T>> buffer, VkDeviceSize range = sizeof(T), VkDeviceSize offset = 0);
private:
  VkDescriptorBufferInfo m_descriptoBuferInfo;

  friend class Write_Descriptor_Set;
};

template<class T>
void Uniform_Buffer_Descriptor::update(core::shared_ptr<Uniform_Buffer<T>> buffer, VkDeviceSize range, VkDeviceSize offset)
{
  m_descriptoBuferInfo.buffer = buffer->getHandle();
  m_descriptoBuferInfo.offset = offset;
  m_descriptoBuferInfo.range  = range;
}

}