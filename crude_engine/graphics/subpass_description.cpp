#include <vulkan/vulkan.hpp>

module crude.graphics.vulkan.subpass_description;

import crude.graphics.vulkan.device;

namespace crude::graphics
{

Subpass_Description::Subpass_Description(VkPipelineBindPoint                                pipelineBindPoint,
                                         const core::span<VkAttachmentReference>&           inputAttachments,
                                         const core::span<VkAttachmentReference>&           colorAttachments,
                                         //!TODO const Array_Unsafe<Attachment_Reference>&  resolveAttachments,
                                         const core::Optional<VkAttachmentReference*>&      depthStencilAttachment,
                                         const core::span<core::uint32>&                    preserveAttachments)
{
  this->flags = 0u;

  this->pipelineBindPoint       = pipelineBindPoint;
  this->inputAttachmentCount    = inputAttachments.size();
  this->colorAttachmentCount    = colorAttachments.size();
  this->preserveAttachmentCount = preserveAttachments.size();

  this->pInputAttachments       = inputAttachments.data();
  this->pColorAttachments       = colorAttachments.data();
  this->pPreserveAttachments    = preserveAttachments.data();

  this->pDepthStencilAttachment = depthStencilAttachment.valueOr(nullptr);

  // !TODO
  this->pResolveAttachments = nullptr;
}

}