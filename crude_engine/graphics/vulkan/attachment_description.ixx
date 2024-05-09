module;

#include <vulkan/vulkan.h>

export module crude_engine.graphics.vulkan.attachment_description;

import crude_engine.core.alias;

export namespace crude_engine
{

// ! sizeof(Attachment_Description) == sizeof(VkAttachmentDescription)
// for render_pass.cpp
struct Attachment_Description final : public VkAttachmentDescription
{
  explicit Attachment_Description(VkFormat               format,
                                  VkSampleCountFlagBits  samples,
                                  VkAttachmentLoadOp     loadOp,
                                  VkAttachmentStoreOp    storeOp,
                                  VkAttachmentLoadOp     stencilLoadOp,
                                  VkAttachmentStoreOp    stencilStoreOp,
                                  VkImageLayout          initialLayout,
                                  VkImageLayout          finalLayout);
};

}