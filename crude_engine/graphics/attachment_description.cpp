#include <vulkan/vulkan.h>

module crude.graphics.attachment_description;

import crude.graphics.swap_chain;

namespace crude::graphics
{

Attachment_Description::Attachment_Description(VkFormat                  format,
                                               VkSampleCountFlagBits     samples,
                                               Attachment_Load_Store_OP  colorOp,
                                               Attachment_Load_Store_OP  stenicilOp,
                                               VkImageLayout             initialLayout,
                                               VkImageLayout             finalLayout)
{
  this->flags = 0u;
  this->format = format;
  this->samples = samples;
  this->loadOp = colorOp.load;
  this->storeOp = colorOp.store;
  this->stencilLoadOp = stenicilOp.load;
  this->stencilStoreOp = stenicilOp.store;
  this->initialLayout = initialLayout;
  this->finalLayout = finalLayout;
}

Swapchain_Attachment_Description::Swapchain_Attachment_Description(core::shared_ptr<Swap_Chain> swapchain)
  :
  Attachment_Description(
    swapchain->getSurfaceFormat().format,
    VK_SAMPLE_COUNT_1_BIT,
    attachment_op::gClearStore,
    attachment_op::gDontCare,
    VK_IMAGE_LAYOUT_UNDEFINED,
    VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
{}


Depth_Attachment_Description::Depth_Attachment_Description(Format format, VkSampleCountFlagBits samples)
  :
  Attachment_Description(
    format,
    samples,
    attachment_op::gClearStore,
    attachment_op::gDontCare,
    VK_IMAGE_LAYOUT_UNDEFINED,
    VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
{}

}