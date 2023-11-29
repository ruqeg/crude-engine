#pragma once

#include "../../core/core.hpp"
#include "include_vulkan.hpp"
#include "object.hpp"

namespace crude_vulkan_01
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
