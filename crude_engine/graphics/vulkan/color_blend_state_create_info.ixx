module;

#include <vulkan/vulkan.hpp>

export module crude_engine.graphics.vulkan.color_blend_state_create_info;

import crude_engine.core.alias;
import crude_engine.core.std_containers_stack;

export namespace crude_engine
{

class Color_Blend_State_Create_Info final : public VkPipelineColorBlendStateCreateInfo
{
public:
  explicit Color_Blend_State_Create_Info(const span<VkPipelineColorBlendAttachmentState>&  attachments,
                                         const array<float32, 4>&                          blendConstants,
                                         VkBool32                                          logicOpEnable,
                                         VkLogicOp                                         logicOp);
};

}
