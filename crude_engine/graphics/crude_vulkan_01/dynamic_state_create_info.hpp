#pragma once

#include "../../core/core.hpp"
#include "include_vulkan.hpp"
#include <vector>

namespace crude_vulkan_01
{

class Dynamic_State_Create_Info : public VkPipelineDynamicStateCreateInfo
{
public:
  explicit Dynamic_State_Create_Info(const std::vector<VkDynamicState>& dynamicStates);
  Dynamic_State_Create_Info(const Dynamic_State_Create_Info& other);
  Dynamic_State_Create_Info(Dynamic_State_Create_Info&& other) noexcept;
  Dynamic_State_Create_Info& operator=(const Dynamic_State_Create_Info& other);
  Dynamic_State_Create_Info& operator=(Dynamic_State_Create_Info&& other) noexcept;
  ~Dynamic_State_Create_Info();
private:
  void copy(const VkDynamicState* opDynamicStates, uint32 odynamicStatesCount);
  void move(Dynamic_State_Create_Info& other);
};

}