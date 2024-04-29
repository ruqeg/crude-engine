#pragma once

#include <core/std_containers.hpp>
#include <graphics/vulkan/include_vulkan.hpp>

namespace crude_engine
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
