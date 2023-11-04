#pragma once

#include "core.hpp"
#include "include_vulkan.hpp"
#include "object.hpp"

namespace crude_vulkan_01 
{

class Input_Assembly_State_Create_Info : public VkPipelineInputAssemblyStateCreateInfo
{
public:
  Input_Assembly_State_Create_Info(VkPrimitiveTopology  topology,
                                   VkBool32             primitiveRestartEnable);
};

}
