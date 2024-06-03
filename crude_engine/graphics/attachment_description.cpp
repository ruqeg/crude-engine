module crude.graphics.attachment_description;

namespace crude::graphics
{

Attachment_Description::Attachment_Description(VkFormat               format,
                                               VkImageLayout          initialLayout,
                                               VkImageLayout          finalLayout,
                                               VkAttachmentLoadOp     loadOp,
                                               VkAttachmentStoreOp    storeOp,
                                               VkSampleCountFlagBits  samples,
                                               VkAttachmentLoadOp     stencilLoadOp,
                                               VkAttachmentStoreOp    stencilStoreOp)
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

}