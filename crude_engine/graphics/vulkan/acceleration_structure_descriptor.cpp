#include <vulkan/vulkan.hpp>

module crude.gfx.vk.acceleration_structure_descriptor;

import crude.gfx.vk.acceleration_structure;

namespace crude::gfx::vk
{

Acceleration_Structure_Descriptor::Acceleration_Structure_Descriptor(core::uint32 binding, VkShaderStageFlags stageFlags)
  : Descriptor_Set_Layout_Binding{ binding, VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR, 1u, stageFlags}
{
  m_writeDescriptorAcceleration.sType                      = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_KHR;
  m_writeDescriptorAcceleration.pNext                      = nullptr;
}

void Acceleration_Structure_Descriptor::write(VkWriteDescriptorSet& writeDescriptorSet)
{
  writeDescriptorSet = {};
  writeDescriptorSet.pNext = &m_writeDescriptorAcceleration;
}

void Acceleration_Structure_Descriptor::update(core::shared_ptr<Acceleration_Structure> accelerationStructure)
{
  m_writeDescriptorAcceleration.pAccelerationStructures    = &accelerationStructure->getHandle();
  m_writeDescriptorAcceleration.accelerationStructureCount = 1u;
}

}