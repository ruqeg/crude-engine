#include <vulkan/vulkan.hpp>

module crude.graphics.multisample_state_create_info;

namespace crude::graphics
{

Multisample_State_Create_Info::Multisample_State_Create_Info(VkSampleCountFlagBits  rasterizationSamples,
                                                             VkBool32               sampleShadingEnable,
                                                             float                  minSampleShading,
                                                             // !TODO const VkSampleMask*  pSampleMask,
                                                             VkBool32               alphaToCoverageEnable,
                                                             VkBool32               alphaToOneEnable)
{
  this->sType                  = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  this->flags                  = 0u;
  this->pNext                  = nullptr;
  this->rasterizationSamples   = rasterizationSamples;
  this->sampleShadingEnable    = sampleShadingEnable;
  this->minSampleShading       = minSampleShading;
  this->pSampleMask            = nullptr;
  this->alphaToCoverageEnable  = alphaToCoverageEnable;
  this->alphaToOneEnable       = alphaToOneEnable;
}

}
