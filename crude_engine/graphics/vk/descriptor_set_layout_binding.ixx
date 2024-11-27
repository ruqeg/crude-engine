module;

#include <vulkan/vulkan.hpp>

export module crude.gfx.vk.descriptor_set_layout_binding;

export import crude.core.alias;

export namespace crude::gfx::vk
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
protected:
  void writeEmpty(VkWriteDescriptorSet& writeDescriptorSet)
  {
    writeDescriptorSet.sType             = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.pNext             = nullptr;
    writeDescriptorSet.dstSet            = nullptr;
    writeDescriptorSet.dstBinding        = 0u;
    writeDescriptorSet.dstArrayElement   = 0u;
    writeDescriptorSet.descriptorCount   = 0u;
    writeDescriptorSet.descriptorType    = VK_DESCRIPTOR_TYPE_MAX_ENUM;
    writeDescriptorSet.pImageInfo        = nullptr;
    writeDescriptorSet.pBufferInfo       = nullptr;
    writeDescriptorSet.pTexelBufferView  = nullptr;
  }
};

}