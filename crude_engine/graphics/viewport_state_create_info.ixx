module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.viewport_state_create_info;

import crude.core.alias;
import crude.core.std_containers_stack;
import crude.core.shared_ptr;

export namespace crude::graphics
{

class Viewport_State_Create_Info : public VkPipelineViewportStateCreateInfo
{
public:
  explicit Viewport_State_Create_Info(const core::span<VkViewport>&  viewports,
                                      const core::span<VkRect2D>&    scissors);
  explicit Viewport_State_Create_Info(core::uint32 viewportsCount, core::uint32 scissorsCount);
};

}
