module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.subpass_dependency;

import crude.core.alias;

export namespace crude::graphics
{

// ! sizeof(Subpass_Dependency) == sizeof(VkSubpassDependency)
// for render_pass.cpp
struct Subpass_Dependency final : public VkSubpassDependency
{
  explicit Subpass_Dependency(core::uint32          srcSubpass,
                              core::uint32          dstSubpass,
                              VkPipelineStageFlags  srcStageMask,
                              VkPipelineStageFlags  dstStageMask,
                              VkAccessFlags         srcAccessMask,
                              VkAccessFlags         dstAccessMask,
                              VkDependencyFlags     dependencyFlags);
};

}