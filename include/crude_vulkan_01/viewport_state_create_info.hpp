#pragma once

#include "core.hpp"
#include "object.hpp"
#include "include_vulkan.hpp"
#include <vector>

namespace crude_vulkan_01
{

class Viewport_State_Create_Info : public VkPipelineViewportStateCreateInfo
{
public:
  Viewport_State_Create_Info(const std::vector<VkViewport>&  viewports,
                             const std::vector<VkRect2D>&    scissors);
  Viewport_State_Create_Info(uint32 viewportsCount,
                             uint32 scissorsCount);
  ~Viewport_State_Create_Info();
};

}
