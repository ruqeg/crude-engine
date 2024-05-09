module;

#include <vulkan/vulkan.hpp>

export module crude_engine.graphics.vulkan.tessellation_state_create_info;

import crude_engine.core.alias;

export namespace crude_engine
{

class Tessellation_State_Create_Info : public VkPipelineTessellationStateCreateInfo
{
public:
  explicit Tessellation_State_Create_Info(uint32 patchControlPoints, VkPipelineTessellationStateCreateFlags flags = 0u);
};

}