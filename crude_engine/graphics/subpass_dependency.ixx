module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.subpass_dependency;

import crude.core.alias;

export namespace crude::graphics
{

class Subpass_Dependency final : public VkSubpassDependency
{
public:
  explicit Subpass_Dependency(core::uint32          srcSubpass,
                              core::uint32          dstSubpass,
                              VkPipelineStageFlags  srcStageMask,
                              VkPipelineStageFlags  dstStageMask,
                              VkAccessFlags         srcAccessMask,
                              VkAccessFlags         dstAccessMask,
                              VkDependencyFlags     dependencyFlags);
};

}