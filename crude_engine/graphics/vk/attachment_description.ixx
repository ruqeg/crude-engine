module;

#include <vulkan/vulkan.h>

export module crude.gfx.vk.attachment_description;

export import crude.gfx.vk.format;
export import crude.gfx.vk.attachment_load_store_op;

export namespace crude::gfx::vk
{

class Attachment_Description : public VkAttachmentDescription
{
public:
  struct Initialize;
public:
  explicit Attachment_Description(const Initialize& info);
};

struct Attachment_Description::Initialize
{
  VkFormat                  format;
  VkSampleCountFlagBits     samples;
  Attachment_Load_Store_OP  colorOp;
  Attachment_Load_Store_OP  stenicilOp;
  VkImageLayout             initialLayout;
  VkImageLayout             finalLayout;
};

Attachment_Description::Attachment_Description(const Initialize& info)
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

}