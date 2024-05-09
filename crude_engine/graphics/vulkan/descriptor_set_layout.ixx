module;

#include <vulkan/vulkan.hpp>

export module crude_engine.graphics.vulkan.descriptor_set_layout;

import crude_engine.core.shared_ptr;
import crude_engine.core.std_containers_stack;
import crude_engine.graphics.vulkan.vulkan_object;

export namespace crude_engine
{

class Device;

class Descriptor_Set_Layout : public Vulkan_Object<VkDescriptorSetLayout>
{
public:
  explicit Descriptor_Set_Layout(Shared_Ptr<const Device>                   device,
                                 const span<VkDescriptorSetLayoutBinding>&  bindings);
  ~Descriptor_Set_Layout();
private:
  Shared_Ptr<const Device>  m_device;
};

}
