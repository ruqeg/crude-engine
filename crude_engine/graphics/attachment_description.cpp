#include <vulkan/vulkan.h>

module crude.graphics.attachment_description;

import crude.graphics.swap_chain;
import crude.graphics.image_attachment;

namespace crude::graphics
{

Attachment_Description::Attachment_Description(const Initialize_Info& info)
{
  this->flags          = 0u;
  this->format         = info.format;
  this->samples        = info.samples;
  this->loadOp         = info.colorOp.load;
  this->storeOp        = info.colorOp.store;
  this->stencilLoadOp  = info.stenicilOp.load;
  this->stencilStoreOp = info.stenicilOp.store;
  this->initialLayout  = info.initialLayout;
  this->finalLayout    = info.finalLayout;
}

Color_Attachment_Description::Color_Attachment_Description(const Initialize_Info& info)
  : Attachment_Description({
    .format        = info.attachment->getFormat(),
    .samples       = info.attachment->getSampleCount(),
    .colorOp       = info.colorOp,
    .stenicilOp    = info.stenicilOp,
    .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    .finalLayout   = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    })
{}

Depth_Attachment_Description::Depth_Attachment_Description(const Initialize_Info& info)
  : Attachment_Description({
    .format        = info.attachment->getFormat(),
    .samples       = info.attachment->getSampleCount(),
    .colorOp       = attachment_op::gClearStore,
    .stenicilOp    = attachment_op::gDontCare,
    .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    .finalLayout   = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
    })
{}

Swapchain_Attachment_Description::Swapchain_Attachment_Description(const Initialize_Info& info)
  : Attachment_Description({
    .format        = info.swapchain->getSurfaceFormat().format,
    .samples       = VK_SAMPLE_COUNT_1_BIT,
    .colorOp       = info.colorOp,
    .stenicilOp    = attachment_op::gDontCare,
    .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    .finalLayout   = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
    })
{}

}