#pragma once

#include "../../core/core.hpp"
#include "include_vulkan.hpp"
#include "object.hpp"
#include "viewport_state_create_info.hpp"
#include <vector>

namespace crude_engine 
{

class Vertex_Input_State_Create_Info : public VkPipelineVertexInputStateCreateInfo
{
public:
  explicit Vertex_Input_State_Create_Info(std::vector<VkVertexInputBindingDescription>&    bindingDescriptions,
                                          std::vector<VkVertexInputAttributeDescription>&  attributeDescriptions);
  Vertex_Input_State_Create_Info(const Vertex_Input_State_Create_Info& other);
  Vertex_Input_State_Create_Info(Vertex_Input_State_Create_Info&& other) noexcept;
  Vertex_Input_State_Create_Info& operator=(const Vertex_Input_State_Create_Info& other);
  Vertex_Input_State_Create_Info& operator=(Vertex_Input_State_Create_Info&& other) noexcept;
  ~Vertex_Input_State_Create_Info();
private:
  void copy(const VkVertexInputBindingDescription*    pBindings,
            const VkVertexInputAttributeDescription*  pAttributes,
            uint32                                    bindingsCount,
            uint32                                    attributesCount);
  void move(Vertex_Input_State_Create_Info& other);
};

}
