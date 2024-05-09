module;

#include <vulkan/vulkan.hpp>

export module crude_engine.graphics.vulkan.dynamic_state_create_info;

import crude_engine.core.std_containers_stack;

export namespace crude_engine
{

class Dynamic_State_Create_Info : public VkPipelineDynamicStateCreateInfo
{
public:
  explicit Dynamic_State_Create_Info(const span<VkDynamicState>& dynamicStates);
};

}