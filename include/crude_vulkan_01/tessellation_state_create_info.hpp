#pragma once

#include "core.hpp"
#include "include_vulkan.hpp"

namespace crude_vulkan_01
{

class Tessellation_State_Create_Info : public VkPipelineTessellationStateCreateInfo
{
public:
  explicit Tessellation_State_Create_Info(uint32 patchControlPoints, VkPipelineTessellationStateCreateFlags flags = 0u);
                                          

};

}