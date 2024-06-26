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
  struct Initialize
  {
    VkSampleCountFlagBits  rasterizationSamples;
    VkBool32               sampleShadingEnable;
    float                  minSampleShading;
    // !TODO const VkSampleMask*  pSampleMask;
    VkBool32               alphaToCoverageEnable;
    VkBool32               alphaToOneEnable;
  };
public:
  explicit Multisample_State_Create_Info(const Initialize& info)
  {
    this->sType                  = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    this->flags                  = 0u;
    this->pNext                  = nullptr;
    this->rasterizationSamples   = info.rasterizationSamples;
    this->sampleShadingEnable    = info.sampleShadingEnable;
    this->minSampleShading       = info.minSampleShading;
    this->pSampleMask            = nullptr;
    this->alphaToCoverageEnable  = info.alphaToCoverageEnable;
    this->alphaToOneEnable       = info.alphaToOneEnable;
  }
};
}
