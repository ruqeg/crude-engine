module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.depth_stencil_state_create_info;

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
                                           core::float32     maxDepthBounds)
  {
    this->sType                  = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    this->flags                  = 0u;
    this->pNext                  = nullptr;
    this->depthTestEnable        = depthTestEnable;
    this->depthWriteEnable       = depthWriteEnable;
    this->depthCompareOp         = depthCompareOp;
    this->depthBoundsTestEnable  = depthBoundsTestEnable;
    this->stencilTestEnable      = stencilTestEnable;
    this->front                  = front;
    this->back                   = back;
    this->minDepthBounds         = minDepthBounds;
    this->maxDepthBounds         = maxDepthBounds;
  }
};

}
