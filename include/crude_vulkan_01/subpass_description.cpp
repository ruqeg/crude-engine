#include "subpass_description.hpp"

namespace crude_vulkan_01
{

Subpass_Description::Subpass_Description(VkPipelineBindPoint                          pipelineBindPoint,
                                         const std::vector<VkAttachmentReference>&    inputAttachments,
                                         const std::vector<VkAttachmentReference>&    colorAttachments,
                                         // !TODO const std::vector<VkAttachmentReference>&  resolveAttachments,
                                         const std::optional<VkAttachmentReference>&  depthStencilAttachment,
                                         const std::vector<uint32>&                   preserveAttachments)
{
  const VkAttachmentReference* opDepthStencilAttachment = depthStencilAttachment.has_value() ? (&depthStencilAttachment.value()) : nullptr;
  copy(pipelineBindPoint, inputAttachments.data(), inputAttachments.size(), colorAttachments.data(), colorAttachments.size(), opDepthStencilAttachment, preserveAttachments.data(), preserveAttachments.size());
}

Subpass_Description::Subpass_Description(const Subpass_Description& other)
{
  copy(other.pipelineBindPoint, other.pInputAttachments, other.inputAttachmentCount, other.pColorAttachments, other.colorAttachmentCount, other.pDepthStencilAttachment, other.pPreserveAttachments, other.preserveAttachmentCount);
}

Subpass_Description& Subpass_Description::operator=(const Subpass_Description& other)
{
  copy(other.pipelineBindPoint, other.pInputAttachments, other.inputAttachmentCount, other.pColorAttachments, other.colorAttachmentCount, other.pDepthStencilAttachment, other.pPreserveAttachments, other.preserveAttachmentCount);
  return *this;
}

Subpass_Description::Subpass_Description(Subpass_Description&& other) noexcept
{
  move(other);
}

Subpass_Description& Subpass_Description::operator=(Subpass_Description&& other) noexcept
{
  move(other);
  return *this;
}

Subpass_Description::~Subpass_Description()
{
  //=========
  // !FREE
  if (pColorAttachments)        CRUDE_VULKAN_01_DELETE[] pColorAttachments;
  if (pInputAttachments)        CRUDE_VULKAN_01_DELETE[] pInputAttachments;
  if (pDepthStencilAttachment)  CRUDE_VULKAN_01_DELETE pDepthStencilAttachment;
  if (pPreserveAttachments)     CRUDE_VULKAN_01_DELETE[] pPreserveAttachments;
  //=========
}

void Subpass_Description::copy(VkPipelineBindPoint           opipelineBindPoint, 
                               const VkAttachmentReference*  opInputAttachments, 
                               uint32                        oinputAttachmentsCount, 
                               const VkAttachmentReference*  opColorAttachments, 
                               uint32                        ocolorAttachmentsCount, 
                               const VkAttachmentReference*  opDepthStencilAttachment, 
                               const uint32*                 opPreserveAttachments,
                               uint32                        opreserveAttachmentsCount)
{
  VkAttachmentReference*  npInputAttachments        = nullptr;
  VkAttachmentReference*  npColorAttachments        = nullptr;
  VkAttachmentReference*  npDepthStencilAttachment  = nullptr;
  uint32*                 npPreserveAttachments     = nullptr;

  //=========
  // !MALLOC
  if (ocolorAttachmentsCount > 0u)     npColorAttachments       = CRUDE_VULKAN_01_NEW VkAttachmentReference[ocolorAttachmentsCount];
  if (oinputAttachmentsCount > 0u)     npInputAttachments       = CRUDE_VULKAN_01_NEW VkAttachmentReference[oinputAttachmentsCount];
  if (opreserveAttachmentsCount > 0u)  npPreserveAttachments    = CRUDE_VULKAN_01_NEW uint32[opreserveAttachmentsCount];
  if (opDepthStencilAttachment)        npDepthStencilAttachment = CRUDE_VULKAN_01_NEW VkAttachmentReference(*opDepthStencilAttachment);
  //=========

  if (ocolorAttachmentsCount > 0u)     CRUDE_VULKAN_01_COPY_MEMORY(npColorAttachments, opColorAttachments, ocolorAttachmentsCount * sizeof(VkAttachmentReference));
  if (oinputAttachmentsCount > 0u)     CRUDE_VULKAN_01_COPY_MEMORY(npInputAttachments, opInputAttachments, oinputAttachmentsCount * sizeof(VkAttachmentReference));
  if (opreserveAttachmentsCount > 0u)  CRUDE_VULKAN_01_COPY_MEMORY(npPreserveAttachments, opPreserveAttachments, opreserveAttachmentsCount * sizeof(uint32));

  this->flags                    = 0u;
  this->pipelineBindPoint        = opipelineBindPoint;
  this->inputAttachmentCount     = oinputAttachmentsCount;
  this->colorAttachmentCount     = ocolorAttachmentsCount;
  this->preserveAttachmentCount  = opreserveAttachmentsCount;
  this->pInputAttachments        = npInputAttachments;
  this->pColorAttachments        = npColorAttachments;
  this->pPreserveAttachments     = npPreserveAttachments;
  this->pDepthStencilAttachment  = npDepthStencilAttachment;
  // !TODO
  this->pResolveAttachments = nullptr;
}

void Subpass_Description::move(Subpass_Description& other)
{
  this->flags                    = 0u;
  this->pipelineBindPoint        = other.pipelineBindPoint;
  this->inputAttachmentCount     = other.inputAttachmentCount;
  this->colorAttachmentCount     = other.colorAttachmentCount;
  this->preserveAttachmentCount  = other.preserveAttachmentCount;

  //=========
  // !MOVE PTR
  this->pInputAttachments        = other.pInputAttachments;
  this->pColorAttachments        = other.pColorAttachments;
  this->pPreserveAttachments     = other.pPreserveAttachments;
  this->pDepthStencilAttachment  = other.pDepthStencilAttachment;
  //=========

  // !TODO
  this->pResolveAttachments = nullptr;

  other.pInputAttachments        = nullptr;
  other.pColorAttachments        = nullptr;
  other.pPreserveAttachments     = nullptr;
  other.pDepthStencilAttachment  = nullptr;
}

}