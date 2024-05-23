module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.color_blend_state_create_info;

import crude.core.alias;
import crude.core.std_containers_stack;

export namespace crude::graphics
{

class Color_Blend_State_Create_Info final : public VkPipelineColorBlendStateCreateInfo
{
public:
  explicit Color_Blend_State_Create_Info(const core::span<VkPipelineColorBlendAttachmentState>&  attachments,
                                         const core::array<core::float32, 4>&                    blendConstants,
                                         VkBool32                                                logicOpEnable,
                                         VkLogicOp                                               logicOp);
};

}
