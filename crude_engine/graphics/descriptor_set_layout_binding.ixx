module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.descriptor_set_layout_binding;

export import crude.core.alias;

export namespace crude::graphics
{

class Descriptor_Set_Layout_Binding : public VkDescriptorSetLayoutBinding
{
public:
  Descriptor_Set_Layout_Binding(core::uint32 binding, VkDescriptorType descriptorType, core::uint32 descriptorCount, VkShaderStageFlags stageFlags)
  {
    this->binding            = binding;
    this->descriptorType     = descriptorType;
    this->descriptorCount    = descriptorCount;
    this->stageFlags         = stageFlags;
    this->pImmutableSamplers = nullptr;
  }
};

}