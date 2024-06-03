module;

#include <vulkan/vulkan.h>

export module crude.graphics.attachment_description;

import crude.core.alias;

export namespace crude::graphics
{

// !TODO sizeof(Attachment_Description) == sizeof(VkAttachmentDescription)
// for render_pass.cpp
struct Attachment_Description final : public VkAttachmentDescription
{
  explicit Attachment_Description(VkFormat               format,
                                  VkImageLayout          initialLayout,
                                  VkImageLayout          finalLayout,
                                  VkAttachmentLoadOp     loadOp,
                                  VkAttachmentStoreOp    storeOp,
                                  VkSampleCountFlagBits  samples        = VK_SAMPLE_COUNT_1_BIT,
                                  VkAttachmentLoadOp     stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                  VkAttachmentStoreOp    stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE);
};

}