module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.subpass_dependency;

import crude.core.alias;

export namespace crude::graphics
{

class Subpass_Dependency final : public VkSubpassDependency
{
public:
  struct Initialize;
public:
  explicit Subpass_Dependency(const Initialize& initialize);
};

struct Subpass_Dependency::Initialize
{
  core::uint32          srcSubpass;
  core::uint32          dstSubpass;
  VkPipelineStageFlags  srcStageMask;
  VkPipelineStageFlags  dstStageMask;
  VkAccessFlags         srcAccessMask;
  VkAccessFlags         dstAccessMask;
  VkDependencyFlags     dependencyFlags;
};

Subpass_Dependency::Subpass_Dependency(const Initialize& initialize)
{
  this->srcSubpass = initialize.srcSubpass;
  this->dstSubpass = initialize.dstSubpass;
  this->srcStageMask = initialize.srcStageMask;
  this->dstStageMask = initialize.dstStageMask;
  this->srcAccessMask = initialize.srcAccessMask;
  this->dstAccessMask = initialize.dstAccessMask;
  this->dependencyFlags = initialize.dependencyFlags;
}

}