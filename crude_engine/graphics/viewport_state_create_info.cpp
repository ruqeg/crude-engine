#include <vulkan/vulkan.hpp>

module crude.graphics.vulkan.viewport_state_create_info;

namespace crude::graphics
{

Viewport_State_Create_Info::Viewport_State_Create_Info(const core::span<VkViewport>&  viewports,
                                                       const core::span<VkRect2D>&    scissors)
{
  this->sType          = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  this->flags          = 0u;
  this->pNext          = 0u;
  this->viewportCount  = viewports.size();
  this->pViewports     = viewports.data();
  this->scissorCount   = scissors.size();
  this->pScissors      = scissors.data();
}

Viewport_State_Create_Info::Viewport_State_Create_Info(core::uint32 viewportsCount, core::uint32 scissorsCount)
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
