module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.vertex_input_state_create_info;

import crude.core.std_containers_stack;

export namespace crude::graphics
{

class Vertex_Input_State_Create_Info : public VkPipelineVertexInputStateCreateInfo
{
public:
  explicit Vertex_Input_State_Create_Info(const core::span<const VkVertexInputBindingDescription>&    bindingDescriptions,
                                          const core::span<const VkVertexInputAttributeDescription>&  attributeDescriptions);
};

}
