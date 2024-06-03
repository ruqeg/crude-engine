#include <vulkan/vulkan.hpp>

module crude.graphics.subpass_description;

import crude.graphics.device;

namespace crude::graphics
{

Subpass_Description::Subpass_Description(VkPipelineBindPoint                                pipelineBindPoint,
                                         const core::span<Attachment_Reference>&            inputAttachments,
                                         const core::span<Attachment_Reference>&            colorAttachments,
                                         //!TODO const Array_Unsafe<Attachment_Reference>&  resolveAttachments,
                                         const Attachment_Reference*                        depthStencilAttachment,
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

  this->pDepthStencilAttachment = depthStencilAttachment;

  // !TODO
  this->pResolveAttachments = nullptr;
}

}