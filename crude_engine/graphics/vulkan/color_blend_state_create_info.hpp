#pragma once

#include <core/data_structures/array_stack.hpp>
#include <graphics/vulkan/include_vulkan.hpp>

namespace crude_engine
{

class Color_Blend_State_Create_Info final : public VkPipelineColorBlendStateCreateInfo
{
public:
  explicit Color_Blend_State_Create_Info(const VkPipelineColorBlendAttachmentState*&  pAttachments,
                                         uint32                                       attachmentCount,
                                         const Array_Stack<float32, 4>&               blendConstants,
                                         VkBool32                                     logicOpEnable,
                                         VkLogicOp                                    logicOp);
};

}
