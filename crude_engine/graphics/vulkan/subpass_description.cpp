#include <graphics/vulkan/subpass_description.hpp>

namespace crude_engine
{

Subpass_Description::Subpass_Description(VkPipelineBindPoint                      pipelineBindPoint,
                                         const span<VkAttachmentReference>&       inputAttachments,
                                         const span<VkAttachmentReference>&       colorAttachments,
                                         //!TODO const Array_Unsafe<Attachment_Reference>&  resolveAttachments,
                                         const Optional<VkAttachmentReference*>&  depthStencilAttachment,
                                         const span<uint32>&                      preserveAttachments)
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