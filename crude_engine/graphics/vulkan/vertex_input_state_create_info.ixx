module;

#include <vulkan/vulkan.hpp>

export module crude_engine.graphics.vulkan.vertex_input_state_create_info;

import crude_engine.core.std_containers_stack;

export namespace crude_engine 
{

class Vertex_Input_State_Create_Info : public VkPipelineVertexInputStateCreateInfo
{
public:
  explicit Vertex_Input_State_Create_Info(const span<VkVertexInputBindingDescription>&    bindingDescriptions,
                                          const span<VkVertexInputAttributeDescription>&  attributeDescriptions);
};

}
