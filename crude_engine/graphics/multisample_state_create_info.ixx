module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.multisample_state_create_info;

import crude.core.std_containers_stack;
import crude.core.std_containers_heap;
import crude.graphics.vulkan_object;
import crude.graphics.application;

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
                                         VkBool32               alphaToOneEnable)
  {
    this->sType                  = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    this->flags                  = 0u;
    this->pNext                  = nullptr;
    this->rasterizationSamples   = rasterizationSamples;
    this->sampleShadingEnable    = sampleShadingEnable;
    this->minSampleShading       = minSampleShading;
    this->pSampleMask            = nullptr;
    this->alphaToCoverageEnable  = alphaToCoverageEnable;
    this->alphaToOneEnable       = alphaToOneEnable;
  }
};
}
