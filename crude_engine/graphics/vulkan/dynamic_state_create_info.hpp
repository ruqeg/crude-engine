#pragma once

#include <graphics/vulkan/include_vulkan.hpp>
#include <core/alias.hpp>

namespace crude_engine
{

class Dynamic_State_Create_Info : public VkPipelineDynamicStateCreateInfo
{
public:
  explicit Dynamic_State_Create_Info(const VkDynamicState* pDynamicStates, uint32 dynamicStateCount);
};

}