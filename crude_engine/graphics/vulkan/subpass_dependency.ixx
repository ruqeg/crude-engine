module;

#include <vulkan/vulkan.hpp>

export module crude_engine.graphics.vulkan.subpass_dependency;

import crude_engine.core.alias;

export namespace crude_engine
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