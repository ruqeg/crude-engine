#include "vertex_input_state_create_info.hpp"

namespace crude_vulkan_01 
{

Vertex_Input_State_Create_Info::Vertex_Input_State_Create_Info(std::vector<VkVertexInputBindingDescription>&    bindingDescriptions,        
                                                               std::vector<VkVertexInputAttributeDescription>&  attributeDescriptions)
{
  const uint32 bindingsSize                         = bindingDescriptions.size();
  const uint32 attributesSize                       = attributeDescriptions.size();
  VkVertexInputBindingDescription*    npBindings    = nullptr;
  VkVertexInputAttributeDescription*  npAttributes  = nullptr;

  //=========
  // !MALLOC
  if (bindingsSize > 0u)    npBindings    = CRUDE_VULKAN_01_NEW VkVertexInputBindingDescription[bindingsSize];
  if (attributesSize > 0u)  npAttributes  = CRUDE_VULKAN_01_NEW VkVertexInputAttributeDescription[attributesSize];
  //=========
  
  if (bindingsSize > 0u)    CRUDE_VULKAN_01_COPY_MEMORY(npBindings, bindingDescriptions.data(), bindingsSize * sizeof(VkVertexInputBindingDescription));
  if (attributesSize > 0u)  CRUDE_VULKAN_01_COPY_MEMORY(npAttributes, attributeDescriptions.data(), attributesSize * sizeof(VkVertexInputAttributeDescription)); 

  this->sType                            = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  this->pNext                            = nullptr;
  this->flags                            = 0u;
  this->vertexBindingDescriptionCount    = bindingsSize;
  this->pVertexBindingDescriptions       = npBindings;
  this->vertexAttributeDescriptionCount  = attributesSize;
  this->pVertexAttributeDescriptions     = npAttributes; 
}

Vertex_Input_State_Create_Info::~Vertex_Input_State_Create_Info()
{
  //=========
  // !FREE
  if (pVertexBindingDescriptions)    CRUDE_VULKAN_01_DELETE[] pVertexBindingDescriptions;
  if (pVertexAttributeDescriptions)  CRUDE_VULKAN_01_DELETE[] pVertexAttributeDescriptions;
  //=========
}

}
