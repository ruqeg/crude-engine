#include <vulkan/vulkan.hpp>

module crude.graphics.dynamic_state_create_info;

namespace crude::graphics
{

Dynamic_State_Create_Info::Dynamic_State_Create_Info(const core::span<VkDynamicState>& dynamicStates)
{
  this->sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  this->pNext             = nullptr;
  this->flags             = 0u;
  this->pDynamicStates    = dynamicStates.data();
  this->dynamicStateCount = dynamicStates.size();
}

}