module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.tessellation_state_create_info;

import crude.core.alias;

export namespace crude::graphics
{

class Tessellation_State_Create_Info : public VkPipelineTessellationStateCreateInfo
{
public:
  explicit Tessellation_State_Create_Info(core::uint32 patchControlPoints, VkPipelineTessellationStateCreateFlags flags = 0u)
  {
    this->sType               = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
    this->pNext               = nullptr;
    this->flags               = flags;
    this->patchControlPoints  = patchControlPoints;
  }
};

}
