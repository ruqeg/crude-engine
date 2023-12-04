#pragma once

#include "../../core/core.hpp"
#include "include_vulkan.hpp"

namespace crude_engine 
{

class Input_Assembly_State_Create_Info : public VkPipelineInputAssemblyStateCreateInfo
{
public:
  explicit Input_Assembly_State_Create_Info(VkPrimitiveTopology  topology,
                                            VkBool32             primitiveRestartEnable);
};

}
