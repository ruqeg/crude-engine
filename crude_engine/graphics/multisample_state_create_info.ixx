module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.vulkan.multisample_state_create_info;

import crude.core.shared_ptr;
import crude.core.std_containers_stack;
import crude.core.std_containers_heap;
import crude.graphics.vulkan.vulkan_object;
import crude.graphics.vulkan.application;

export namespace crude::graphics
{

class Multisample_State_Create_Info : public VkPipelineMultisampleStateCreateInfo
{
public:
  explicit Multisample_State_Create_Info(VkSampleCountFlagBits  rasterizationSamples,
                                         VkBool32               sampleShadingEnable,
                                         float                  minSampleShading,
                                         // !TODO const VkSampleMask*  pSampleMask,
                                         VkBool32               alphaToCoverageEnable,
                                         VkBool32               alphaToOneEnable);
};
}
