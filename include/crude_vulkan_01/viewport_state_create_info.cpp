#include "viewport_state_create_info.hpp"
  
namespace crude_vulkan_01
{

Viewport_State_Create_Info::Viewport_State_Create_Info(const std::vector<VkViewport>&  viewports,
                                                       const std::vector<VkRect2D>&    scissors)
{
  const uint32 viewportsCount  = viewports.size();
  const uint32 scissorsCount   = scissors.size();
  VkViewport* npViewports      = nullptr;
  VkRect2D*   npScissors       = nullptr;
  const uint32 viewportsbsize  = viewportCount * sizeof(VkViewport);
  const uint32 scissorsbsize   = scissorsCount * sizeof(VkRect2D);

  //=========
  // !MALLOC
  if (viewportsCount > 0u)  npViewports = CRUDE_VULKAN_01_NEW VkViewport[viewportsCount]; 
  if (scissorsCount > 0u)   npScissors  = CRUDE_VULKAN_01_NEW VkRect2D[scissorsCount];
  //=========

  if (viewportsCount > 0u)  CRUDE_VULKAN_01_COPY_MEMORY(npViewports, viewports.data(), viewportsbsize); 
  if (scissorsCount > 0u)   CRUDE_VULKAN_01_COPY_MEMORY(npScissors, scissors.data(), scissorsbsize);

  this->sType          = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  this->flags          = 0u;
  this->pNext          = 0u;
  this->viewportCount  = viewportsCount;
  this->pViewports     = npViewports;
  this->scissorCount   = scissorsCount;
  this->pScissors      = npScissors;
}
  
Viewport_State_Create_Info::Viewport_State_Create_Info(uint32 viewportsCount,
                                                       uint32 scissorsCount)
{
  this->sType          = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  this->flags          = 0u;
  this->pNext          = 0u;
  this->viewportCount  = viewportsCount;
  this->pViewports     = nullptr;
  this->scissorCount   = scissorsCount;
  this->pScissors      = nullptr;
}

Viewport_State_Create_Info::~Viewport_State_Create_Info()
{
  //=========
  // !FREE
  if (this->pViewports)  CRUDE_VULKAN_01_DELETE [] this->pViewports;
  if (this->pScissors)   CRUDE_VULKAN_01_DELETE [] this->pScissors;
  //=========
}

}
