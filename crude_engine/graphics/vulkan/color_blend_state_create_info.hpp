#pragma once

#include <core/array_unsafe.hpp>
#include <core/array_stack.hpp>
#include <graphics/vulkan/include_vulkan.hpp>

namespace crude_engine
{

class Color_Blend_State_Create_Info final : public VkPipelineColorBlendStateCreateInfo
{
public:
  explicit Color_Blend_State_Create_Info(const crude_engine::Array_Unsafe<VkPipelineColorBlendAttachmentState>&  attachments,
                                         const Array_Stack<float32, 4>&                                          blendConstants,
                                         VkBool32                                                                logicOpEnable,
                                         VkLogicOp                                                               logicOp);
};

}
