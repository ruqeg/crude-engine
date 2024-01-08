#pragma once

#include <core/data_structures/array_unsafe.hpp>
#include <graphics/vulkan/viewport_state_create_info.hpp>

namespace crude_engine 
{

class Vertex_Input_State_Create_Info : public VkPipelineVertexInputStateCreateInfo
{
public:
  explicit Vertex_Input_State_Create_Info(Array_Unsafe<VkVertexInputBindingDescription>&    bindingDescriptions,
                                          Array_Unsafe<VkVertexInputAttributeDescription>&  attributeDescriptions);
};

}
