#include "subpass_description.hpp"

namespace crude_engine
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
  this->flags                    = 0u;
  this->pipelineBindPoint        = opipelineBindPoint;
  this->inputAttachmentCount     = oinputAttachmentsCount;
  this->colorAttachmentCount     = ocolorAttachmentsCount;
  this->preserveAttachmentCount  = opreserveAttachmentsCount;
  //=========
  // !MALLOC
  this->pInputAttachments        = CRUDE_VULKAN_01_NEW_COPY_MEMORY(VkAttachmentReference, opInputAttachments, oinputAttachmentsCount);
  this->pColorAttachments        = CRUDE_VULKAN_01_NEW_COPY_MEMORY(VkAttachmentReference, opColorAttachments, ocolorAttachmentsCount);
  this->pPreserveAttachments     = CRUDE_VULKAN_01_NEW_COPY_MEMORY(uint32, opPreserveAttachments, opreserveAttachmentsCount);
  this->pDepthStencilAttachment  = opDepthStencilAttachment ? CRUDE_VULKAN_01_NEW VkAttachmentReference(*opDepthStencilAttachment) : nullptr;
  //=========
  this->pResolveAttachments = nullptr; // !TODO
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