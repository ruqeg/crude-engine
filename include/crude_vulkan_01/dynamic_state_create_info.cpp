#include "dynamic_state_create_info.hpp"

namespace crude_vulkan_01
{

Dynamic_State_Create_Info::Dynamic_State_Create_Info(const std::vector<VkDynamicState>& dynamicStates)
{
  copy(dynamicStates.data(), dynamicStates.size());
}

Dynamic_State_Create_Info::Dynamic_State_Create_Info(const Dynamic_State_Create_Info& other)
{
  copy(other.pDynamicStates, other.dynamicStateCount);
}

Dynamic_State_Create_Info::Dynamic_State_Create_Info(Dynamic_State_Create_Info&& other) noexcept
{
  move(other);
}

Dynamic_State_Create_Info& Dynamic_State_Create_Info::operator=(const Dynamic_State_Create_Info& other)
{
  copy(other.pDynamicStates, other.dynamicStateCount);
  return *this;
}

Dynamic_State_Create_Info& Dynamic_State_Create_Info::operator=(Dynamic_State_Create_Info&& other) noexcept
{
  move(other);
  return *this;
}

Dynamic_State_Create_Info::~Dynamic_State_Create_Info()
{
  //=========
  // !FREE
  CRUDE_VULKAN_01_DELETE[] this->pDynamicStates;
  //=========
}

void Dynamic_State_Create_Info::copy(const VkDynamicState* opDynamicStates, uint32 odynamicStatesCount)
{
  VkDynamicState* npDynamicStates  = nullptr;

  //=========
  // !MALLOC
  if (odynamicStatesCount > 0u)  npDynamicStates = CRUDE_VULKAN_01_NEW VkDynamicState[odynamicStatesCount];
  //=========
  
  if (odynamicStatesCount > 0u)  CRUDE_VULKAN_01_COPY_MEMORY(npDynamicStates, opDynamicStates, odynamicStatesCount);

  this->sType           = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  this->pNext           = nullptr;
  this->flags           = 0u;
  this->pDynamicStates  = npDynamicStates;
}

void Dynamic_State_Create_Info::move(Dynamic_State_Create_Info& other)
{
  this->sType           = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  this->pNext           = nullptr;
  this->flags           = 0u;
  this->pDynamicStates  = other.pDynamicStates;

  other.pDynamicStates  = nullptr;
}

}