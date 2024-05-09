module;

#include <vulkan/vulkan.hpp>

export module crude_engine.graphics.vulkan.multisample_state_create_info;

import crude_engine.core.shared_ptr;
import crude_engine.core.std_containers_stack;
import crude_engine.core.std_containers_heap;
import crude_engine.graphics.vulkan.vulkan_object;
import crude_engine.graphics.vulkan.application;

export namespace crude_engine
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
