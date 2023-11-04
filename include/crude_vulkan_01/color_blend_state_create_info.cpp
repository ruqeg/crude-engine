#include "color_blend_state_create_info.hpp"
#include "core.hpp"

namespace crude_vulkan_01
{

Color_Blend_State_Create_Info::Color_Blend_State_Create_Info(VkBool32                                                 logicOpEnable,
                                                             VkLogicOp                                                logicOp,
                                                             const std::vector<VkPipelineColorBlendAttachmentState>&  attachments,
                                                             const std::array<float32, 4>&                            blendConstants)
{
  copy(attachments.data(), attachments.size(), blendConstants.data(), logicOpEnable, logicOp);
}
  
Color_Blend_State_Create_Info::Color_Blend_State_Create_Info(const Color_Blend_State_Create_Info& other)
{
  copy(other.pAttachments, other.attachmentCount, other.blendConstants, other.logicOpEnable, other.logicOp);
}

Color_Blend_State_Create_Info::Color_Blend_State_Create_Info(Color_Blend_State_Create_Info&& other)
{
  move(other);
}
  
Color_Blend_State_Create_Info& Color_Blend_State_Create_Info::operator=(const Color_Blend_State_Create_Info& other)
{
  copy(other.pAttachments, other.attachmentCount, other.blendConstants, other.logicOpEnable, other.logicOp);
  return *this;
}
  
Color_Blend_State_Create_Info& Color_Blend_State_Create_Info::operator=(Color_Blend_State_Create_Info&& other)
{
  move(other);
  return *this;
}
  
Color_Blend_State_Create_Info::~Color_Blend_State_Create_Info()
{
  //=========
  // !MALLOC
  if (pAttachments)  CRUDE_VULKAN_01_DELETE[] pAttachments;
  //=========
}

void Color_Blend_State_Create_Info::copy(const VkPipelineColorBlendAttachmentState* pAttachments,
                                         uint32                                     attachmentsCount,
                                         const float32*                             pBlendConstants,
                                         VkBool32                                   logicOpEnable,
                                         VkLogicOp                                  logicOp)
{
  const uint32 attachmentsbsize                       = attachmentsCount * sizeof(VkPipelineColorBlendAttachmentState);
  VkPipelineColorBlendAttachmentState* npAttachments  = nullptr;

  //=========
  // !MALLOC
  if (attachmentsCount > 0u)  npAttachments  = CRUDE_VULKAN_01_NEW VkPipelineColorBlendAttachmentState[attachmentsCount];
  //=========
  
  if (attachmentsCount > 0u)  CRUDE_VULKAN_01_COPY_MEMORY(npAttachments, pAttachments, attachmentsbsize); 

  this->sType                            = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  this->pNext                            = nullptr;
  this->flags                            = 0u;
  this->pAttachments                     = npAttachments;
  this->attachmentCount                  = attachmentCount;
  this->blendConstants[0]                = pBlendConstants[0];
  this->blendConstants[1]                = pBlendConstants[1];
  this->blendConstants[2]                = pBlendConstants[2];
  this->blendConstants[3]                = pBlendConstants[3];
  this->logicOpEnable                    = logicOpEnable;
  this->logicOp                          = logicOp;
}

void Color_Blend_State_Create_Info::move(Color_Blend_State_Create_Info& other)
{
  this->sType                            = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  this->pNext                            = nullptr;
  this->flags                            = 0u;
  this->pAttachments                     = other.pAttachments;
  this->attachmentCount                  = other.attachmentCount;
  this->blendConstants[0]                = other.blendConstants[0];
  this->blendConstants[1]                = other.blendConstants[1];
  this->blendConstants[2]                = other.blendConstants[2];
  this->blendConstants[3]                = other.blendConstants[3];
  this->logicOpEnable                    = other.logicOpEnable;
  this->logicOp                          = other.logicOp;
  other.pAttachments                     = nullptr;
}

}
