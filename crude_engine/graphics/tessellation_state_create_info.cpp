#include <vulkan/vulkan.hpp>

module crude.graphics.tessellation_state_create_info;

namespace crude::graphics
{

Tessellation_State_Create_Info::Tessellation_State_Create_Info(core::uint32 patchControlPoints, VkPipelineTessellationStateCreateFlags flags)
{
  this->sType               = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
  this->pNext               = nullptr;
  this->flags               = flags;
  this->patchControlPoints  = patchControlPoints;
}

}
