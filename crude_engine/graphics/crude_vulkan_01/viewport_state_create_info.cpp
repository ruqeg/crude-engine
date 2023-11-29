#include "viewport_state_create_info.hpp"
#include "../../core/types.hpp"
  
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

Viewport_State_Create_Info::Viewport_State_Create_Info(Viewport_State_Create_Info&& other) noexcept
{
  move(other);
}

Viewport_State_Create_Info& Viewport_State_Create_Info::operator=(const Viewport_State_Create_Info& other)
{
  copy(other.pViewports, other.pScissors, other.viewportCount, other.scissorCount);
  return *this;
}

Viewport_State_Create_Info& Viewport_State_Create_Info::operator=(Viewport_State_Create_Info&& other) noexcept
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
  this->sType          = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  this->flags          = 0u;
  this->pNext          = 0u;
  //=========
  // !MALLOC
  this->pViewports     = CRUDE_VULKAN_01_NEW_COPY_MEMORY(VkViewport, pViewports, viewportCount);
  this->pScissors      = CRUDE_VULKAN_01_NEW_COPY_MEMORY(VkRect2D, pScissors, scissorCount);
  //=========
  this->viewportCount  = viewportCount;
  this->scissorCount   = scissorCount;
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
