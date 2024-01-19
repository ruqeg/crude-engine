#pragma once

#include <core/data_structures/array_unsafe.hpp>
#include <graphics/vulkan/include_vulkan.hpp>

namespace crude_engine
{

class Viewport_State_Create_Info : public VkPipelineViewportStateCreateInfo
{
public:
  explicit Viewport_State_Create_Info(const Array_Unsafe<VkViewport>&  viewports,
                                      const Array_Unsafe<VkRect2D>&    scissors);
  explicit Viewport_State_Create_Info(uint32 viewportsCount, uint32 scissorsCount);
};

}
