module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.subpass_description;

import crude.core.shared_ptr;
import crude.core.optional;
import crude.core.std_containers_stack;

export namespace crude::graphics
{

class Device;

struct Attachment_Reference : public VkAttachmentReference
{
  Attachment_Reference(core::uint32 attachment, VkImageLayout layout)
  {
    this->attachment = attachment;
    this->layout = layout;
  }
};

// ! sizeof(Subpass_Description) == sizeof(VkSubpassDescription)
// for render_pass.cpp
struct Subpass_Description final : public VkSubpassDescription
{
  explicit Subpass_Description(VkPipelineBindPoint                                pipelineBindPoint,
                               const core::span<Attachment_Reference>&            inputAttachments,
                               const core::span<Attachment_Reference>&            colorAttachments,
                               //!TODO const Array_Unsafe<Attachment_Reference>&  resolveAttachments,
                               const Attachment_Reference*                        depthStencilAttachment,
                               const core::span<core::uint32>&                    preserveAttachments);
};

}