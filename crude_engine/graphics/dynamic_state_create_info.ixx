module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.vulkan.dynamic_state_create_info;

import crude.core.std_containers_stack;

export namespace crude::graphics
{

class Dynamic_State_Create_Info : public VkPipelineDynamicStateCreateInfo
{
public:
  explicit Dynamic_State_Create_Info(const core::span<VkDynamicState>& dynamicStates);
};

}