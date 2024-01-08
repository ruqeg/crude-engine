#pragma once

#include <graphics/vulkan/include_vulkan.hpp>

namespace crude_engine
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
