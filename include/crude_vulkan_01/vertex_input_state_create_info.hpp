#pragma once

#include "core.hpp"
#include "include_vulkan.hpp"
#include "object.hpp"
#include <vector>

namespace crude_vulkan_01 
{

class Vertex_Input_State_Create_Info : public VkPipelineVertexInputStateCreateInfo
{
public:
  explicit Vertex_Input_State_Create_Info(std::vector<VkVertexInputBindingDescription>&    bindingDescriptions,
                                          std::vector<VkVertexInputAttributeDescription>&  attributeDescriptions);
  ~Vertex_Input_State_Create_Info();
};

}
