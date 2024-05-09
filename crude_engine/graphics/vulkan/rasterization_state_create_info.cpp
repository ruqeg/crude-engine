#include <vulkan/vulkan.hpp>

module crude_engine.graphics.vulkan.rasterization_state_create_info;

namespace crude_engine 
{
  
Rasterization_State_Create_Info::Rasterization_State_Create_Info(VkBool32         depthClampEnable,
                                                                 VkBool32         rasterizerDiscardEnable,
                                                                 VkPolygonMode    polygonMode,
                                                                 VkCullModeFlags  cullMode,
                                                                 VkFrontFace      frontFace,
                                                                 VkBool32         depthBiasEnable,
                                                                 float            depthBiasConstantFactor,
                                                                 float            depthBiasClamp,
                                                                 float            depthBiasSlopeFactor,
                                                                 float            lineWidth)
{
  this->sType                     = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  this->flags                     = 0u;
  this->pNext                     = nullptr;

  this->depthClampEnable          = depthClampEnable;
  this->rasterizerDiscardEnable   = rasterizerDiscardEnable;
  this->polygonMode               = polygonMode;
  this->cullMode                  = cullMode;
  this->frontFace                 = frontFace;
  this->depthBiasEnable           = depthBiasEnable;
  this->depthBiasConstantFactor   = depthBiasConstantFactor;
  this->depthBiasClamp            = depthBiasClamp;
  this->depthBiasSlopeFactor      = depthBiasSlopeFactor;
  this->lineWidth                 = lineWidth;
}

}
