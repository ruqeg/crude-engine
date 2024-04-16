#pragma once

#include <core/array_unsafe.hpp>
#include <graphics/vulkan/include_vulkan.hpp>

namespace crude_engine
{

class Dynamic_State_Create_Info : public VkPipelineDynamicStateCreateInfo
{
public:
  explicit Dynamic_State_Create_Info(const Array_Unsafe<VkDynamicState>& dynamicStates);
};

}