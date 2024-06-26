module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.dynamic_state_create_info;

import crude.core.std_containers_stack;

export namespace crude::graphics
{

class Dynamic_State_Create_Info : public VkPipelineDynamicStateCreateInfo
{
public:
  explicit Dynamic_State_Create_Info(core::span<const VkDynamicState> dynamicStates)
  {
    this->sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    this->pNext             = nullptr;
    this->flags             = 0u;
    this->pDynamicStates    = dynamicStates.data();
    this->dynamicStateCount = dynamicStates.size();
  }
};

}
