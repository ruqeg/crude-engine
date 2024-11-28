module;

#include <vulkan/vulkan.hpp>

export module crude.gfx.vk.pipeline_layout;

export import crude.core.std_containers_stack;
export import crude.core.std_containers_heap;
export import crude.gfx.vk.push_constant_range;
import crude.gfx.vk.vulkan_object;

export namespace crude::gfx::vk
{

class Device;
class Descriptor_Set_Layout;

class Pipeline_Layout : public Vulkan_Object<VkPipelineLayout>
{
public:
  explicit Pipeline_Layout(core::shared_ptr<const Device>                 device,
                           core::shared_ptr<const Descriptor_Set_Layout>  descriptorSetLayout);
  explicit Pipeline_Layout(core::shared_ptr<const Device>                 device,
                           core::shared_ptr<const Descriptor_Set_Layout>  descriptorSetLayout,
                           const Push_Constant_Range_Base&                pushConstantRange);
  explicit Pipeline_Layout(core::shared_ptr<const Device>                                      device,
                           const core::vector<core::shared_ptr<const Descriptor_Set_Layout>>&  descriptorSetLayouts,
                           const core::vector<Push_Constant_Range_Base>&                       pushConstantRanges = {});
  ~Pipeline_Layout();
private:
  core::vector<core::shared_ptr<const Descriptor_Set_Layout>>  m_setLayouts;
  core::vector<Push_Constant_Range_Base>                       m_pushConstantRanges;
  core::shared_ptr<const Device>                               m_device;
  friend class Command_Buffer;
};

}