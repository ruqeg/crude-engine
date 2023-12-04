#include "tessellation_state_create_info.hpp"

namespace crude_engine
{

Tessellation_State_Create_Info::Tessellation_State_Create_Info(uint32 patchControlPoints, VkPipelineTessellationStateCreateFlags flags)
{
  this->sType               = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
  this->pNext               = nullptr;
  this->flags               = flags;
  this->patchControlPoints  = patchControlPoints;
}

}
