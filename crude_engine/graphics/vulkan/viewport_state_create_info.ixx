module;

#include <vulkan/vulkan.hpp>

export module crude_engine.graphics.vulkan.viewport_state_create_info;

import crude_engine.core.alias;
import crude_engine.core.std_containers_stack;
import crude_engine.core.shared_ptr;

export namespace crude_engine
{

class Viewport_State_Create_Info : public VkPipelineViewportStateCreateInfo
{
public:
  explicit Viewport_State_Create_Info(const span<VkViewport>&  viewports,
                                      const span<VkRect2D>&    scissors);
  explicit Viewport_State_Create_Info(uint32 viewportsCount, uint32 scissorsCount);
};

}
