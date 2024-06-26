module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.depth_stencil_state_create_info;

import crude.core.alias;

export namespace crude::graphics
{

class Depth_Stencil_State_Create_Info : public VkPipelineDepthStencilStateCreateInfo
{
public:
  struct Initialize
  {
    VkBool32          depthTestEnable;
    VkBool32          depthWriteEnable;
    VkCompareOp       depthCompareOp;
    VkBool32          depthBoundsTestEnable;
    VkBool32          stencilTestEnable;
    VkStencilOpState  front;
    VkStencilOpState  back;
    core::float32     minDepthBounds;
    core::float32     maxDepthBounds;
  };
public:
  explicit Depth_Stencil_State_Create_Info(const Initialize& info)
  {
    this->sType                  = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    this->flags                  = 0u;
    this->pNext                  = nullptr;
    this->depthTestEnable        = info.depthTestEnable;
    this->depthWriteEnable       = info.depthWriteEnable;
    this->depthCompareOp         = info.depthCompareOp;
    this->depthBoundsTestEnable  = info.depthBoundsTestEnable;
    this->stencilTestEnable      = info.stencilTestEnable;
    this->front                  = info.front;
    this->back                   = info.back;
    this->minDepthBounds         = info.minDepthBounds;
    this->maxDepthBounds         = info.maxDepthBounds;
  }
};

}
