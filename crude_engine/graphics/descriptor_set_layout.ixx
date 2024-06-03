module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.descriptor_set_layout;

import crude.core.alias;
import crude.core.shared_ptr;
import crude.core.std_containers_stack;
import crude.graphics.vulkan_object;

export namespace crude::graphics
{

class Device;

// !TODO
struct Descriptor_Set_Layout_Binding final : public VkDescriptorSetLayoutBinding
{
  Descriptor_Set_Layout_Binding(core::uint32 binding, VkDescriptorType descriptorType, core::uint32 descriptorCount, VkShaderStageFlags stageFlags)
  {
    this->binding             = binding;
    this->descriptorType      = descriptorType;
    this->descriptorCount     = descriptorCount;
    this->stageFlags          = stageFlags;
    this->pImmutableSamplers  = nullptr;
  }
};

class Descriptor_Set_Layout : public Vulkan_Object<VkDescriptorSetLayout>
{
public:
  explicit Descriptor_Set_Layout(core::Shared_Ptr<const Device>                    device,
                                 const core::span<Descriptor_Set_Layout_Binding>&  bindings);
  ~Descriptor_Set_Layout();
private:
  core::Shared_Ptr<const Device>  m_device;
};

}
