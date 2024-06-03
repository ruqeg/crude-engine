#include <vulkan/vulkan.hpp>

module crude.graphics.vertex_input_state_create_info;

namespace crude::graphics
{

Vertex_Input_State_Create_Info::Vertex_Input_State_Create_Info(const core::span<const VkVertexInputBindingDescription>&    bindingDescriptions,
                                                               const core::span<const VkVertexInputAttributeDescription>&  attributeDescriptions)
{

  this->sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  this->pNext                           = nullptr;
  this->flags                           = 0u;
  this->vertexAttributeDescriptionCount = attributeDescriptions.size();
  this->pVertexAttributeDescriptions    = attributeDescriptions.data();
  this->vertexBindingDescriptionCount   = bindingDescriptions.size();
  this->pVertexBindingDescriptions      = bindingDescriptions.data();
}

}
