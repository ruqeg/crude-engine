module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.vulkan.pipeline_layout;

import crude.core.shared_ptr;
import crude.core.std_containers_stack;
import crude.graphics.vulkan.vulkan_object;

export namespace crude::graphics
{

class Device;
class Descriptor_Set_Layout;

class Pipeline_Layout : public Vulkan_Object<VkPipelineLayout>
{
public:
  explicit Pipeline_Layout(core::Shared_Ptr<const Device>                              device,
                           const core::span<core::Shared_Ptr<Descriptor_Set_Layout>>&  descriptorSetLayouts,
                           const core::span<VkPushConstantRange>&                      pushConstantRanges = {});
  ~Pipeline_Layout();
private:
  core::Shared_Ptr<const Device>  m_device;
};

}