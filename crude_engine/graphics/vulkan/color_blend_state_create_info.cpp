#include <graphics/vulkan/color_blend_state_create_info.hpp>

namespace crude_engine
{

Color_Blend_State_Create_Info::Color_Blend_State_Create_Info(const crude_engine::Array_Unsafe<VkPipelineColorBlendAttachmentState>&  attachments,
                                                             const Array_Stack<float32, 4>&                                          blendConstants,
                                                             VkBool32                                                                logicOpEnable,
                                                             VkLogicOp                                                               logicOp)
{
  this->sType              = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  this->pNext              = nullptr;
  this->flags              = 0u;

  this->pAttachments       = attachments.data();
  this->attachmentCount    = static_cast<uint32>(attachments.size());
  this->blendConstants[0]  = blendConstants[0];
  this->blendConstants[1]  = blendConstants[1];
  this->blendConstants[2]  = blendConstants[2];
  this->blendConstants[3]  = blendConstants[3];
  this->logicOpEnable      = logicOpEnable;
  this->logicOp            = logicOp;
}
}
