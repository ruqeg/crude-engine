#pragma once

#include <core/std_containers.hpp>
#include <graphics/vulkan/include_vulkan.hpp>

namespace crude_engine
{

class Dynamic_State_Create_Info : public VkPipelineDynamicStateCreateInfo
{
public:
  explicit Dynamic_State_Create_Info(const span<VkDynamicState>& dynamicStates);
};

}