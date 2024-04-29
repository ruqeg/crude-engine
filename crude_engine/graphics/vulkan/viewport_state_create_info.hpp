#pragma once

#include <core/std_containers.hpp>
#include <graphics/vulkan/include_vulkan.hpp>

namespace crude_engine
{

class Viewport_State_Create_Info : public VkPipelineViewportStateCreateInfo
{
public:
  explicit Viewport_State_Create_Info(const span<VkViewport>&  viewports,
                                      const span<VkRect2D>&    scissors);
  explicit Viewport_State_Create_Info(uint32 viewportsCount, uint32 scissorsCount);
};

}
