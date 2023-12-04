#include "vertex_input_state_create_info.hpp"

namespace crude_engine 
{

Vertex_Input_State_Create_Info::Vertex_Input_State_Create_Info(std::vector<VkVertexInputBindingDescription>&    bindingDescriptions,        
                                                               std::vector<VkVertexInputAttributeDescription>&  attributeDescriptions)
{
  copy(bindingDescriptions.data(), attributeDescriptions.data(), bindingDescriptions.size(), attributeDescriptions.size());
}
  
Vertex_Input_State_Create_Info::Vertex_Input_State_Create_Info(const Vertex_Input_State_Create_Info& other)
{
  copy(other.pVertexBindingDescriptions, other.pVertexAttributeDescriptions, other.vertexBindingDescriptionCount, other.vertexAttributeDescriptionCount);
}

Vertex_Input_State_Create_Info::Vertex_Input_State_Create_Info(Vertex_Input_State_Create_Info&& other) noexcept
{
  move(other);
}

Vertex_Input_State_Create_Info& Vertex_Input_State_Create_Info::operator=(const Vertex_Input_State_Create_Info& other)
{
  copy(other.pVertexBindingDescriptions, other.pVertexAttributeDescriptions, other.vertexBindingDescriptionCount, other.vertexAttributeDescriptionCount);
  return *this;
}

Vertex_Input_State_Create_Info& Vertex_Input_State_Create_Info::operator=(Vertex_Input_State_Create_Info&& other) noexcept
{
  move(other);
  return *this;
}

Vertex_Input_State_Create_Info::~Vertex_Input_State_Create_Info()
{
  //=========
  // !FREE
  if (pVertexBindingDescriptions)    CRUDE_VULKAN_01_DELETE[] pVertexBindingDescriptions;
  if (pVertexAttributeDescriptions)  CRUDE_VULKAN_01_DELETE[] pVertexAttributeDescriptions;
  //=========
}
  
void Vertex_Input_State_Create_Info::copy(const VkVertexInputBindingDescription*    pBindings,
                                          const VkVertexInputAttributeDescription*  pAttributes,
                                          uint32                                    bindingsCount,
                                          uint32                                    attributesCount)
{
  this->sType                            = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  this->pNext                            = nullptr;
  this->flags                            = 0u;
  //=========
  // !MALLOC
  this->pVertexBindingDescriptions       = CRUDE_VULKAN_01_NEW_COPY_MEMORY(VkVertexInputBindingDescription, pBindings, bindingsCount);
  this->pVertexAttributeDescriptions     = CRUDE_VULKAN_01_NEW_COPY_MEMORY(VkVertexInputAttributeDescription, pAttributes, attributesCount);
  //=========
  this->vertexBindingDescriptionCount    = bindingsCount;
  this->vertexAttributeDescriptionCount  = attributesCount;
}
  
void Vertex_Input_State_Create_Info::move(Vertex_Input_State_Create_Info& other)
{
  this->sType                            = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  this->pNext                            = nullptr;
  this->flags                            = 0u;
  this->vertexAttributeDescriptionCount  = other.vertexAttributeDescriptionCount;
  this->pVertexAttributeDescriptions     = other.pVertexAttributeDescriptions;
  this->vertexBindingDescriptionCount    = other.vertexBindingDescriptionCount;
  this->pVertexBindingDescriptions       = other.pVertexBindingDescriptions;
  other.pVertexBindingDescriptions       = nullptr;
  other.pVertexAttributeDescriptions     = nullptr;
}

}
