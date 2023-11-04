#include "viewport_state_create_info.hpp"
#include "types.hpp"
  
namespace crude_vulkan_01
{

Viewport_State_Create_Info::Viewport_State_Create_Info(const std::vector<VkViewport>&  viewports,
                                                       const std::vector<VkRect2D>&    scissors)
{
  copy(viewports.data(), scissors.data(), viewports.size(), scissors.size());
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
  
Viewport_State_Create_Info::Viewport_State_Create_Info(const Viewport_State_Create_Info& other)
{
  copy(other.pViewports, other.pScissors, other.viewportCount, other.scissorCount);
}

Viewport_State_Create_Info::Viewport_State_Create_Info(Viewport_State_Create_Info&& other)
{
  move(other);
}

Viewport_State_Create_Info& Viewport_State_Create_Info::operator=(const Viewport_State_Create_Info& other)
{
  copy(other.pViewports, other.pScissors, other.viewportCount, other.scissorCount);
  return *this;
}

Viewport_State_Create_Info& Viewport_State_Create_Info::operator=(Viewport_State_Create_Info&& other)
{
  move(other);
  return *this;
}

Viewport_State_Create_Info::~Viewport_State_Create_Info()
{
  //=========
  // !FREE
  if (this->pViewports)  CRUDE_VULKAN_01_DELETE [] this->pViewports;
  if (this->pScissors)   CRUDE_VULKAN_01_DELETE [] this->pScissors;
  //=========
}
  
void Viewport_State_Create_Info::copy(const VkViewport*  pViewports,
                                      const VkRect2D*    pScissors,
                                      uint32             viewportCount,
                                      uint32             scissorCount)
{
  const uint32 viewportsCount  = viewportCount;
  const uint32 scissorsCount   = scissorCount;
  VkViewport* npViewports      = nullptr;
  VkRect2D*   npScissors       = nullptr;
  const uint32 viewportsbsize  = viewportsCount * sizeof(VkViewport);
  const uint32 scissorsbsize   = scissorsCount * sizeof(VkRect2D);

  //=========
  // !MALLOC
  if (viewportsCount > 0u)  npViewports = CRUDE_VULKAN_01_NEW VkViewport[viewportsCount]; 
  if (scissorsCount > 0u)   npScissors  = CRUDE_VULKAN_01_NEW VkRect2D[scissorsCount];
  //=========

  if (viewportsCount > 0u)  CRUDE_VULKAN_01_COPY_MEMORY(npViewports, pViewports, viewportsbsize); 
  if (scissorsCount > 0u)   CRUDE_VULKAN_01_COPY_MEMORY(npScissors, pScissors, scissorsbsize);

  this->sType          = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  this->flags          = 0u;
  this->pNext          = 0u;
  this->viewportCount  = viewportsCount;
  this->pViewports     = npViewports;
  this->scissorCount   = scissorsCount;
  this->pScissors      = npScissors;
}

void Viewport_State_Create_Info::move(Viewport_State_Create_Info& other)
{
  this->sType          = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  this->flags          = 0u;
  this->pNext          = 0u;
  this->viewportCount  = other.viewportCount;
  this->pViewports     = other.pViewports;
  this->scissorCount   = other.scissorCount;
  this->pScissors      = other.pScissors;
  other.pScissors      = nullptr;
  other.pViewports     = nullptr;
}

}
