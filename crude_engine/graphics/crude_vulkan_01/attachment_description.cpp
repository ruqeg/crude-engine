#include "attachment_description.hpp"

namespace crude_vulkan_01
{

Attachment_Description::Attachment_Description(VkFormat               format,
                                               VkSampleCountFlagBits  samples,
                                               VkAttachmentLoadOp     loadOp,
                                               VkAttachmentStoreOp    storeOp,
                                               VkAttachmentLoadOp     stencilLoadOp,
                                               VkAttachmentStoreOp    stencilStoreOp,
                                               VkImageLayout          initialLayout,
                                               VkImageLayout          finalLayout)
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