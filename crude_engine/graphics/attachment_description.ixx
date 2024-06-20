module;

#include <vulkan/vulkan.h>

export module crude.graphics.attachment_description;

export import crude.graphics.attachment_load_store_op;

export namespace crude::graphics
{

class Attachment_Description : public VkAttachmentDescription
{
public:
  explicit Attachment_Description(VkFormat                  format,
                                  VkSampleCountFlagBits     samples,
                                  Attachment_Load_Store_OP  colorOp,
                                  Attachment_Load_Store_OP  stenicilOp,
                                  VkImageLayout             initialLayout,
                                  VkImageLayout             finalLayout)
  {
    this->flags           = 0u;
    this->format          = format;
    this->samples         = samples;
    this->loadOp          = colorOp.load;
    this->storeOp         = colorOp.store;
    this->stencilLoadOp   = stenicilOp.load;
    this->stencilStoreOp  = stenicilOp.store;
    this->initialLayout   = initialLayout;
    this->finalLayout     = finalLayout;
  }
};

}