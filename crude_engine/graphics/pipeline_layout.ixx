module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.pipeline_layout;

import crude.core.std_containers_stack;
import crude.core.std_containers_heap;
import crude.graphics.vulkan_object;

export namespace crude::graphics
{

class Device;
class Descriptor_Set_Layout;

class Pipeline_Layout : public Vulkan_Object<VkPipelineLayout>
{
public:
  explicit Pipeline_Layout(core::shared_ptr<const Device>                                      device,
                           const core::vector<core::shared_ptr<const Descriptor_Set_Layout>>&  descriptorSetLayouts,
                           const core::vector<VkPushConstantRange>&                            pushConstantRanges = {});
  ~Pipeline_Layout();
private:
  core::vector<core::shared_ptr<const Descriptor_Set_Layout>>  m_setLayouts;
  core::vector<VkPushConstantRange>                            m_pushConstantRanges;
  core::shared_ptr<const Device>                               m_device;
};

}