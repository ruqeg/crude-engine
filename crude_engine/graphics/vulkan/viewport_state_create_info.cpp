#include <graphics/vulkan/viewport_state_create_info.hpp>
  
namespace crude_engine
{

Viewport_State_Create_Info::Viewport_State_Create_Info(const Array_Unsafe<VkViewport>&  viewports,
                                                       const Array_Unsafe<VkRect2D>&    scissors)
{
  this->sType          = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  this->flags          = 0u;
  this->pNext          = 0u;
  this->viewportCount  = viewports.size();
  this->pViewports     = viewports.data();
  this->scissorCount   = scissors.size();
  this->pScissors      = scissors.data();
}

Viewport_State_Create_Info::Viewport_State_Create_Info(uint32 viewportsCount, uint32 scissorsCount)
{
  this->sType          = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  this->flags          = 0u;
  this->pNext          = 0u;
  this->viewportCount  = viewportsCount;
  this->pViewports     = nullptr;
  this->scissorCount   = scissorsCount;
  this->pScissors      = nullptr;
}

}
