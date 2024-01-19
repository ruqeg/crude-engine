#include <graphics/vulkan/vertex_input_state_create_info.hpp>

namespace crude_engine 
{

Vertex_Input_State_Create_Info::Vertex_Input_State_Create_Info(const Array_Unsafe<VkVertexInputBindingDescription>&    bindingDescriptions,
                                                               const Array_Unsafe<VkVertexInputAttributeDescription>&  attributeDescriptions)
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
