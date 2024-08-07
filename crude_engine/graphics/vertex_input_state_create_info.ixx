module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.vertex_input_state_create_info;

import crude.core.std_containers_stack;

export namespace crude::graphics
{

class Vertex_Input_State_Create_Info : public VkPipelineVertexInputStateCreateInfo
{
public:
  struct Initialize
  {
    core::span<const VkVertexInputBindingDescription>   bindings {};
    core::span<const VkVertexInputAttributeDescription> attributes {};
  };
public:
  explicit Vertex_Input_State_Create_Info(const Initialize& info)
  {
    this->sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    this->pNext                           = nullptr;
    this->flags                           = 0u;
    this->vertexAttributeDescriptionCount = info.attributes.size();
    this->pVertexAttributeDescriptions    = info.attributes.data();
    this->vertexBindingDescriptionCount   = info.bindings.size();
    this->pVertexBindingDescriptions      = info.bindings.data();
  }
};

}
