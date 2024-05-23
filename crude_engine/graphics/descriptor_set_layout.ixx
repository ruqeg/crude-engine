module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.descriptor_set_layout;

import crude.core.shared_ptr;
import crude.core.std_containers_stack;
import crude.graphics.vulkan_object;

export namespace crude::graphics
{

class Device;

class Descriptor_Set_Layout : public Vulkan_Object<VkDescriptorSetLayout>
{
public:
  explicit Descriptor_Set_Layout(core::Shared_Ptr<const Device>                   device,
                                 const core::span<VkDescriptorSetLayoutBinding>&  bindings);
  ~Descriptor_Set_Layout();
private:
  core::Shared_Ptr<const Device>  m_device;
};

}
