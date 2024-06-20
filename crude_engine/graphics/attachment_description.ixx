module;

#include <vulkan/vulkan.h>

export module crude.graphics.attachment_description;

import crude.core.alias;

export namespace crude::graphics
{

class Attachment_Description final : public VkAttachmentDescription
{
public:
  explicit Attachment_Description(VkFormat               format,
                                  VkImageLayout          initialLayout,
                                  VkImageLayout          finalLayout,
                                  VkAttachmentLoadOp     loadOp,
                                  VkAttachmentStoreOp    storeOp,
                                  VkSampleCountFlagBits  samples        = VK_SAMPLE_COUNT_1_BIT,
                                  VkAttachmentLoadOp     stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                  VkAttachmentStoreOp    stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE)
  {
    this->flags           = 0u;
    this->format          = format;
    this->samples         = samples;
    this->loadOp          = loadOp;
    this->storeOp         = storeOp;
    this->stencilLoadOp   = stencilLoadOp;
    this->stencilStoreOp  = stencilStoreOp;
    this->initialLayout   = initialLayout;
    this->finalLayout     = finalLayout;
  }
};

}