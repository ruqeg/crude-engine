#include <vulkan/vulkan.hpp>

module crude.graphics.subpass_dependency;

namespace crude::graphics
{
  
Subpass_Dependency::Subpass_Dependency(core::uint32          srcSubpass,
                                       core::uint32          dstSubpass,
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