#include <vulkan/vulkan.hpp>

module crude.graphics.buffer_descriptor;

namespace crude::graphics
{

Uniform_Buffer_Descriptor::Uniform_Buffer_Descriptor(core::uint32 binding, VkShaderStageFlags stageFlags)
  :
  Descriptor_Set_Layout_Binding(binding, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1u, stageFlags) 
{}

}