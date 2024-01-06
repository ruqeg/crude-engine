#pragma once

#include <core/alias.hpp>
#include <graphics/vulkan/include_vulkan.hpp>

namespace crude_engine
{

class Depth_Stencil_State_Create_Info : public VkPipelineDepthStencilStateCreateInfo
{
public:
  explicit Depth_Stencil_State_Create_Info(VkBool32          depthTestEnable,
                                           VkBool32          depthWriteEnable,
                                           VkCompareOp       depthCompareOp,
                                           VkBool32          depthBoundsTestEnable,
                                           VkBool32          stencilTestEnable,
                                           VkStencilOpState  front,
                                           VkStencilOpState  back,
                                           float32           minDepthBounds,
                                           float32           maxDepthBounds);
};

}
