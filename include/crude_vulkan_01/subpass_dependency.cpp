#include "subpass_dependency.hpp"

namespace crude_vulkan_01 
{
  
Subpass_Dependency::Subpass_Dependency(uint32                srcSubpass,
                                       uint32                dstSubpass,
                                       VkPipelineStageFlags  srcStageMask,
                                       VkPipelineStageFlags  dstStageMask,
                                       VkAccessFlags         srcAccessMask,
                                       VkAccessFlags         dstAccessMask,
                                       VkDependencyFlags     dependencyFlags)
{
  this->srcSubpass       = srcSubpass;
  this->dstSubpass       = dstSubpass;
  this->srcStageMask     = srcStageMask;
  this->dstStageMask     = dstStageMask;
  this->srcAccessMask    = srcAccessMask;
  this->dstAccessMask    = dstAccessMask;
  this->dependencyFlags  = dependencyFlags;
}

}