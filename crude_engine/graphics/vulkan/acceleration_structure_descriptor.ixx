module;

#include <vulkan/vulkan.hpp>

export module crude.gfx.vk.acceleration_structure_descriptor;

export import crude.gfx.vk.descriptor_set_layout_binding;
export import crude.core.std_containers_heap;

export namespace crude::gfx::vk
{

class Acceleration_Structure;

class Acceleration_Structure_Descriptor : public Descriptor_Set_Layout_Binding
{
public:
  explicit Acceleration_Structure_Descriptor(core::uint32 binding, VkShaderStageFlags stageFlags);
  void write(VkWriteDescriptorSet& writeDescriptorSet);
  void update(core::shared_ptr<Acceleration_Structure> accelerationStructure);
private:
  VkWriteDescriptorSetAccelerationStructureKHR m_writeDescriptorAcceleration;
};

}