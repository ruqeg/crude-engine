#pragma once

#include "../../core/core.hpp"
#include "object.hpp"
#include "include_vulkan.hpp"
#include <vector>
#include <vulkan/vulkan_core.h>

namespace crude_engine
{

class Viewport_State_Create_Info : public VkPipelineViewportStateCreateInfo
{
public:
  explicit Viewport_State_Create_Info(const std::vector<VkViewport>&  viewports,
                                      const std::vector<VkRect2D>&    scissors);
  explicit Viewport_State_Create_Info(uint32 viewportsCount,
                                      uint32 scissorsCount);
  Viewport_State_Create_Info(const Viewport_State_Create_Info& other);
  Viewport_State_Create_Info(Viewport_State_Create_Info&& other) noexcept;
  Viewport_State_Create_Info& operator=(const Viewport_State_Create_Info& other);
  Viewport_State_Create_Info& operator=(Viewport_State_Create_Info&& other) noexcept;
  ~Viewport_State_Create_Info();
private:
  void copy(const VkViewport*  pViewports,
            const VkRect2D*    pScissors,
            uint32             viewportCount,
            uint32             scissorCount);
  void move(Viewport_State_Create_Info& other);
};

}
