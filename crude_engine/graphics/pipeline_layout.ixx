module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.pipeline_layout;

export import crude.core.std_containers_stack;
export import crude.core.std_containers_heap;
export import crude.graphics.push_constant_range;
import crude.graphics.vulkan_object;

export namespace crude::graphics
{

class Device;
class Descriptor_Set_Layout;

class Pipeline_Layout : public Vulkan_Object<VkPipelineLayout>
{
public:
  explicit Pipeline_Layout(core::shared_ptr<const Device>                             device,
                  const core::vector<core::shared_ptr<const Descriptor_Set_Layout>>&  descriptorSetLayouts,
                  const core::vector<Push_Constant_Range_Base>&                       pushConstantRanges = {});
  ~Pipeline_Layout();
private:
  core::vector<core::shared_ptr<const Descriptor_Set_Layout>>  m_setLayouts;
  core::vector<Push_Constant_Range_Base>                       m_pushConstantRanges;
  core::shared_ptr<const Device>                               m_device;
};

}