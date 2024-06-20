module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.color_blend_state_create_info;

import crude.core.alias;
import crude.core.std_containers_stack;

export namespace crude::graphics
{

struct Pipeline_Color_Blend_Attachment_State : VkPipelineColorBlendAttachmentState
{
  Pipeline_Color_Blend_Attachment_State() = default;
  Pipeline_Color_Blend_Attachment_State(VkBool32                 blendEnable,
                                        VkColorComponentFlags    colorWriteMask,
                                        VkBlendFactor            srcColorBlendFactor,
                                        VkBlendFactor            dstColorBlendFactor,
                                        VkBlendOp                colorBlendOp,
                                        VkBlendFactor            srcAlphaBlendFactor,
                                        VkBlendFactor            dstAlphaBlendFactor,
                                        VkBlendOp                alphaBlendOp)
  {
    this->blendEnable          = blendEnable;
    this->srcColorBlendFactor  = srcColorBlendFactor;
    this->dstColorBlendFactor  = dstColorBlendFactor;
    this->colorBlendOp         = colorBlendOp;
    this->srcAlphaBlendFactor  = srcAlphaBlendFactor;
    this->dstAlphaBlendFactor  = dstAlphaBlendFactor;
    this->alphaBlendOp         = alphaBlendOp;
    this->colorWriteMask       = colorWriteMask;
  }
};

class Color_Blend_State_Create_Info final : public VkPipelineColorBlendStateCreateInfo
{
public:
  explicit Color_Blend_State_Create_Info(const core::span<Pipeline_Color_Blend_Attachment_State>&  attachments,
                                         const core::array<core::float32, 4>&                      blendConstants,
                                         VkBool32                                                  logicOpEnable,
                                         VkLogicOp                                                 logicOp)
  {
    this->sType              = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    this->pNext              = nullptr;
    this->flags              = 0u;

    this->pAttachments       = attachments.data();
    this->attachmentCount    = static_cast<core::uint32>(attachments.size());
    this->blendConstants[0]  = blendConstants[0];
    this->blendConstants[1]  = blendConstants[1];
    this->blendConstants[2]  = blendConstants[2];
    this->blendConstants[3]  = blendConstants[3];
    this->logicOpEnable      = logicOpEnable;
    this->logicOp            = logicOp;
  }
};

}
