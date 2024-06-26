#include <vulkan/vulkan.hpp>

module crude.graphics.subpass_description;

import crude.core.algorithms;
import crude.graphics.device;
import crude.core.memory_manager;

namespace crude::graphics
{

Subpass_Description::Subpass_Description() noexcept
  : Subpass_Description(VK_PIPELINE_BIND_POINT_GRAPHICS)
{}

Subpass_Description::~Subpass_Description() noexcept
{
  if (this->inputAttachmentCount > 0)
    core::defaultCxxDeallocate(this->inputAttachmentCount, this->pInputAttachments);
  if (this->colorAttachmentCount > 0)
    core::defaultCxxDeallocate(this->colorAttachmentCount, this->pColorAttachments);
  if (this->pResolveAttachments != nullptr)
    core::defaultCxxDeallocate(this->pResolveAttachments);
  if (this->pDepthStencilAttachment != nullptr)
    core::defaultCxxDeallocate(this->pDepthStencilAttachment);
  if (this->preserveAttachmentCount > 0)
    core::defaultCxxDeallocate(this->preserveAttachmentCount, this->pPreserveAttachments);
}

Subpass_Description::Subpass_Description(Subpass_Description& other) noexcept
{
  this->flags                   = other.flags;
  this->pipelineBindPoint       = other.pipelineBindPoint;
  this->inputAttachmentCount    = other.inputAttachmentCount;
  this->colorAttachmentCount    = other.colorAttachmentCount;
  this->preserveAttachmentCount = other.preserveAttachmentCount;

  this->pInputAttachments       = core::cxxDefaultAllocateCopy(other.pInputAttachments, other.inputAttachmentCount);
  this->pColorAttachments       = core::cxxDefaultAllocateCopy(other.pColorAttachments, other.colorAttachmentCount);
  this->pPreserveAttachments    = core::cxxDefaultAllocateCopy(other.pPreserveAttachments, other.preserveAttachmentCount);
  this->pResolveAttachments     = core::cxxDefaultAllocateCopy(other.pResolveAttachments, 1u);
  this->pDepthStencilAttachment = core::cxxDefaultAllocateCopy(other.pDepthStencilAttachment, 1u);
}

Subpass_Description::Subpass_Description(Subpass_Description&& other) noexcept
{
  this->flags                   = other.flags;
  this->pipelineBindPoint       = other.pipelineBindPoint;
  this->inputAttachmentCount    = other.inputAttachmentCount;
  this->pInputAttachments       = other.pInputAttachments;
  this->colorAttachmentCount    = other.colorAttachmentCount;
  this->pColorAttachments       = other.pColorAttachments;
  this->pResolveAttachments     = other.pResolveAttachments;
  this->pDepthStencilAttachment = other.pDepthStencilAttachment;
  this->preserveAttachmentCount = other.preserveAttachmentCount;
  this->pPreserveAttachments    = other.pPreserveAttachments;
  
  other.inputAttachmentCount    = 0;
  other.pInputAttachments       = nullptr;
  other.colorAttachmentCount    = 0;
  other.pColorAttachments       = nullptr;
  other.pResolveAttachments     = nullptr;
  other.pDepthStencilAttachment = nullptr;
  other.preserveAttachmentCount = 0;
  other.pPreserveAttachments    = nullptr;
}

Subpass_Description& Subpass_Description::operator=(Subpass_Description& other) noexcept
{
  if (this != &other)
  {
    this->flags                   = other.flags;
    this->pipelineBindPoint       = other.pipelineBindPoint;
    this->inputAttachmentCount    = other.inputAttachmentCount;
    this->colorAttachmentCount    = other.colorAttachmentCount;
    this->preserveAttachmentCount = other.preserveAttachmentCount;

    this->pInputAttachments       = core::cxxDefaultAllocateCopy(other.pInputAttachments, other.inputAttachmentCount);
    this->pColorAttachments       = core::cxxDefaultAllocateCopy(other.pColorAttachments, other.colorAttachmentCount);
    this->pPreserveAttachments    = core::cxxDefaultAllocateCopy(other.pPreserveAttachments, other.preserveAttachmentCount);
    this->pResolveAttachments     = core::cxxDefaultAllocateCopy(other.pResolveAttachments, 1u);
    this->pDepthStencilAttachment = core::cxxDefaultAllocateCopy(other.pDepthStencilAttachment, 1u);
  }

  return *this;
}

Subpass_Description& Subpass_Description::operator=(Subpass_Description&& other) noexcept
{
  this->flags                   = other.flags;
  this->pipelineBindPoint       = other.pipelineBindPoint;
  this->inputAttachmentCount    = other.inputAttachmentCount;
  this->pInputAttachments       = other.pInputAttachments;
  this->colorAttachmentCount    = other.colorAttachmentCount;
  this->pColorAttachments       = other.pColorAttachments;
  this->pResolveAttachments     = other.pResolveAttachments;
  this->pDepthStencilAttachment = other.pDepthStencilAttachment;
  this->preserveAttachmentCount = other.preserveAttachmentCount;
  this->pPreserveAttachments    = other.pPreserveAttachments;
  
  other.inputAttachmentCount    = 0;
  other.pInputAttachments       = nullptr;
  other.colorAttachmentCount    = 0;
  other.pColorAttachments       = nullptr;
  other.pResolveAttachments     = nullptr;
  other.pDepthStencilAttachment = nullptr;
  other.preserveAttachmentCount = 0;
  other.pPreserveAttachments    = nullptr;

  return *this;
}

Subpass_Description::Subpass_Description(VkPipelineBindPoint pipelineBindPoint) noexcept
{
  this->flags                   = 0;
  this->pipelineBindPoint       = pipelineBindPoint;
  this->inputAttachmentCount    = 0;
  this->pInputAttachments       = nullptr;
  this->colorAttachmentCount    = 0;
  this->pColorAttachments       = nullptr;
  this->pResolveAttachments     = nullptr;
  this->pDepthStencilAttachment = nullptr;
  this->preserveAttachmentCount = 0;
  this->pPreserveAttachments    = nullptr;
}

Subpass_Description::Subpass_Description(VkImageLayout colorLayout, VkImageLayout depthStencilLayout, VkImageLayout resolveLayout) noexcept
  : Subpass_Description(VK_PIPELINE_BIND_POINT_GRAPHICS)
{
  if (colorLayout != VK_IMAGE_LAYOUT_UNDEFINED)
  {
    VkAttachmentReference* colorReference = core::defaultCxxAllocate<VkAttachmentReference>();
    if (colorReference)
    {
      colorReference->attachment = 0;
      colorReference->layout = colorLayout;
    }
    this->pColorAttachments = colorReference;
    this->colorAttachmentCount = 1;
  }

  VkAttachmentReference* depthStencilReference = core::defaultCxxAllocate<VkAttachmentReference>();
  if (depthStencilReference)
  {
    depthStencilReference->attachment = 1;
    depthStencilReference->layout = depthStencilLayout;
  }
  this->pDepthStencilAttachment = depthStencilReference;


  VkAttachmentReference* resolveReference = core::defaultCxxAllocate<VkAttachmentReference>();
  if (resolveReference)
  {
    resolveReference->attachment = 2;
    resolveReference->layout = resolveLayout;
  }
  this->pResolveAttachments = resolveReference;
}

Subpass_Description::Subpass_Description(core::span<const VkImageLayout> colorLayouts, VkImageLayout depthStencilLayout) noexcept
  : Subpass_Description(VK_PIPELINE_BIND_POINT_GRAPHICS)
{
  if (!colorLayouts.empty())
  {
    VkAttachmentReference* colorReferences = core::defaultCxxAllocate<VkAttachmentReference>(colorLayouts.size());
    if (colorReferences)
    {
      for (auto layout : colorLayouts)
      {
        colorReferences[this->colorAttachmentCount].attachment = this->colorAttachmentCount;
        colorReferences[this->colorAttachmentCount].layout = layout;
        ++this->colorAttachmentCount;
      }
    }
    this->pColorAttachments = colorReferences;
  }

  VkAttachmentReference* depthStencilReference = core::defaultCxxAllocate<VkAttachmentReference>();
  if (depthStencilReference)
  {
    depthStencilReference->attachment = 1;
    depthStencilReference->layout = depthStencilLayout;
  }
  this->pDepthStencilAttachment = depthStencilReference;
}

}