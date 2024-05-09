module;

#include <vulkan/vulkan.hpp>

export module crude_engine.graphics.vulkan.pipeline_layout;

import crude_engine.core.shared_ptr;
import crude_engine.core.std_containers_stack;
import crude_engine.graphics.vulkan.vulkan_object;

namespace crude_engine
{

class Device;
class Descriptor_Set_Layout;

class Pipeline_Layout : public Vulkan_Object<VkPipelineLayout>
{
public:
  explicit Pipeline_Layout(Shared_Ptr<const Device>                        device,
                           const span<Shared_Ptr<Descriptor_Set_Layout>>&  descriptorSetLayouts,
                           const span<VkPushConstantRange>&                pushConstantRanges = {});
  ~Pipeline_Layout();
private:
  Shared_Ptr<const Device>  m_device;
};

}