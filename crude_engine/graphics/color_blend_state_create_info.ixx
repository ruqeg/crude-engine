module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.color_blend_state_create_info;

import crude.core.alias;
import crude.core.std_containers_stack;

export namespace crude::graphics
{

struct Pipeline_Color_Blend_Attachment_State : VkPipelineColorBlendAttachmentState
{
public:
  struct Initialize
  {
    VkBool32                 blendEnable;
    VkColorComponentFlags    colorWriteMask;
    VkBlendFactor            srcColorBlendFactor;
    VkBlendFactor            dstColorBlendFactor;
    VkBlendOp                colorBlendOp;
    VkBlendFactor            srcAlphaBlendFactor;
    VkBlendFactor            dstAlphaBlendFactor;
    VkBlendOp                alphaBlendOp;
  };
public:
  Pipeline_Color_Blend_Attachment_State() = default;
  explicit Pipeline_Color_Blend_Attachment_State(const Initialize& info)
  {
    this->blendEnable          = info.blendEnable;
    this->srcColorBlendFactor  = info.srcColorBlendFactor;
    this->dstColorBlendFactor  = info.dstColorBlendFactor;
    this->colorBlendOp         = info.colorBlendOp;
    this->srcAlphaBlendFactor  = info.srcAlphaBlendFactor;
    this->dstAlphaBlendFactor  = info.dstAlphaBlendFactor;
    this->alphaBlendOp         = info.alphaBlendOp;
    this->colorWriteMask       = info.colorWriteMask;
  }
};

class Color_Blend_State_Create_Info final : public VkPipelineColorBlendStateCreateInfo
{
public:
  struct Initialize
  {
    core::span<const  Pipeline_Color_Blend_Attachment_State>  attachments;
    core::array<core::float32, 4>                             blendConstants;
    VkBool32                                                  logicOpEnable;
    VkLogicOp                                                 logicOp;
  };
public:
  explicit Color_Blend_State_Create_Info(const Initialize& info)
  {
    this->sType              = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    this->pNext              = nullptr;
    this->flags              = 0u;

    this->pAttachments       = info.attachments.data();
    this->attachmentCount    = static_cast<core::uint32>(info.attachments.size());
    this->blendConstants[0]  = info.blendConstants[0];
    this->blendConstants[1]  = info.blendConstants[1];
    this->blendConstants[2]  = info.blendConstants[2];
    this->blendConstants[3]  = info.blendConstants[3];
    this->logicOpEnable      = info.logicOpEnable;
    this->logicOp            = info.logicOp;
  }
};

}
