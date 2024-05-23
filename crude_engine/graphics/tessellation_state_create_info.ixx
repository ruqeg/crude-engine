module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.tessellation_state_create_info;

import crude.core.alias;

export namespace crude::graphics
{

class Tessellation_State_Create_Info : public VkPipelineTessellationStateCreateInfo
{
public:
  explicit Tessellation_State_Create_Info(core::uint32 patchControlPoints, VkPipelineTessellationStateCreateFlags flags = 0u);
};

}