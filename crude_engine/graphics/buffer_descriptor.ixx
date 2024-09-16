module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.buffer_descriptor;

export import crude.graphics.descriptor_set_layout_binding;
export import crude.core.std_containers_heap;

export namespace crude::graphics
{

class Storage_Buffer;
class Buffer;
template<class T>
class Uniform_Buffer;

class Buffer_Descriptor : public Descriptor_Set_Layout_Binding
{
public:
  void write(VkWriteDescriptorSet& writeDescriptorSet);
  void updateBase(core::shared_ptr<const Buffer> buffer, VkDeviceSize range, VkDeviceSize offset);
  void clear();
protected:
  explicit Buffer_Descriptor(core::uint32 binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags);
private:
  VkDescriptorBufferInfo m_descriptoBuferInfo;
};

class Storage_Buffer_Descriptor : public Buffer_Descriptor
{
public:
  explicit Storage_Buffer_Descriptor(core::uint32 binding, VkShaderStageFlags stageFlags);
  void update(core::shared_ptr<Storage_Buffer> buffer, VkDeviceSize range, VkDeviceSize offset = 0);
};

class Uniform_Buffer_Descriptor : public Buffer_Descriptor
{
public:
  explicit Uniform_Buffer_Descriptor(core::uint32 binding, VkShaderStageFlags stageFlags);
  template<class T>
  void update(core::shared_ptr<Uniform_Buffer<T>> buffer, VkDeviceSize range = sizeof(T), VkDeviceSize offset = 0)
  {
    updateBase(buffer, range, offset);
  }
};

}