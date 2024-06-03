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
                                         VkLogicOp                                                 logicOp);
};

}
