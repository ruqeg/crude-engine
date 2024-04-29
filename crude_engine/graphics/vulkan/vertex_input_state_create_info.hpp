#pragma once

#include <core/std_containers.hpp>
#include <graphics/vulkan/viewport_state_create_info.hpp>

namespace crude_engine 
{

class Vertex_Input_State_Create_Info : public VkPipelineVertexInputStateCreateInfo
{
public:
  explicit Vertex_Input_State_Create_Info(const span<VkVertexInputBindingDescription>&    bindingDescriptions,
                                          const span<VkVertexInputAttributeDescription>&  attributeDescriptions);
};

}
