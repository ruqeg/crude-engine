#pragma once

#include "core.hpp"
#include "include_vulkan.hpp"

namespace crude_vulkan_01
{

// ! sizeof(Subpass_Dependency) == sizeof(VkSubpassDependency)
// for render_pass.cpp
struct Subpass_Dependency final : public VkSubpassDependency
{
  explicit Subpass_Dependency(uint32                srcSubpass,
                              uint32                dstSubpass,
                              VkPipelineStageFlags  srcStageMask,
                              VkPipelineStageFlags  dstStageMask,
                              VkAccessFlags         srcAccessMask,
                              VkAccessFlags         dstAccessMask,
                              VkDependencyFlags     dependencyFlags);
};

}