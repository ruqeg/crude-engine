#include <vulkan/vulkan.h>

module crude.graphics.attachment_description;

import crude.graphics.swap_chain;
import crude.graphics.image_attachment;

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

Color_Attachment_Description::Color_Attachment_Description(core::shared_ptr<Color_Attachment>  attachment,
                                                           Attachment_Load_Store_OP            colorOp,
                                                           Attachment_Load_Store_OP            stenicilOp)
  :
  Attachment_Description(
    attachment->getFormat(),
    attachment->getSampleCount(),
    colorOp,
    stenicilOp,
    VK_IMAGE_LAYOUT_UNDEFINED,
    VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
{}

Depth_Attachment_Description::Depth_Attachment_Description(core::shared_ptr<Depth_Stencil_Attachment> attachment)
  :
  Attachment_Description(
    attachment->getFormat(),
    attachment->getSampleCount(),
    attachment_op::gClearStore,
    attachment_op::gDontCare,
    VK_IMAGE_LAYOUT_UNDEFINED,
    VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
{}

Swapchain_Attachment_Description::Swapchain_Attachment_Description(core::shared_ptr<Swap_Chain> swapchain, Attachment_Load_Store_OP colorOp)
  :
  Attachment_Description(
    swapchain->getSurfaceFormat().format,
    VK_SAMPLE_COUNT_1_BIT,
    colorOp,
    attachment_op::gDontCare,
    VK_IMAGE_LAYOUT_UNDEFINED,
    VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
{}

}