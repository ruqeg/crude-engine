module;

#include <vulkan/vulkan.hpp>

export module crude.gfx.vk.descriptor_set_layout;

export import crude.gfx.vk.descriptor_set_layout_binding;
export import crude.core.std_containers_heap;
export import crude.core.std_containers_stack;
import crude.gfx.vk.vulkan_object;

export namespace crude::gfx::vk
{

class Device;

class Descriptor_Set_Layout : public Vulkan_Object<VkDescriptorSetLayout>
{
public:
  explicit Descriptor_Set_Layout(core::shared_ptr<const Device>                   device,
                                 core::span<const Descriptor_Set_Layout_Binding>  bindings,
                                 bool                                             pushDescriptor = false);
  ~Descriptor_Set_Layout();
private:
  core::shared_ptr<const Device>  m_device;
};

}
