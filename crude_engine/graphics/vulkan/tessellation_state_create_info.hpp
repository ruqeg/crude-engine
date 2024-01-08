#pragma once

#include <core/alias.hpp>
#include <graphics/vulkan/include_vulkan.hpp>

namespace crude_engine
{

class Tessellation_State_Create_Info : public VkPipelineTessellationStateCreateInfo
{
public:
  explicit Tessellation_State_Create_Info(uint32 patchControlPoints, VkPipelineTessellationStateCreateFlags flags = 0u);
};

}