module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.vulkan.depth_stencil_state_create_info;

import crude.core.alias;

export namespace crude::graphics
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
                                           core::float32     minDepthBounds,
                                           core::float32     maxDepthBounds);
};

}
