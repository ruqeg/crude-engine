#include <graphics/vulkan/dynamic_state_create_info.hpp>

namespace crude_engine
{

Dynamic_State_Create_Info::Dynamic_State_Create_Info(const VkDynamicState* pDynamicStates, uint32 dynamicStateCount)
{
  this->sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  this->pNext             = nullptr;
  this->flags             = 0u;
  this->pDynamicStates    = pDynamicStates;
  this->dynamicStateCount = dynamicStateCount;
}

}