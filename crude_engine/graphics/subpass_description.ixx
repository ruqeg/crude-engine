module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.subpass_description;

import crude.core.shared_ptr;
import crude.core.optional;
import crude.core.std_containers_stack;

export namespace crude::graphics
{

class Device;

// ! sizeof(Subpass_Description) == sizeof(VkSubpassDescription)
// for render_pass.cpp
struct Subpass_Description final : public VkSubpassDescription
{
  explicit Subpass_Description(VkPipelineBindPoint                                  pipelineBindPoint,
                               const core::span<VkAttachmentReference>&             inputAttachments,
                               const core::span<VkAttachmentReference>&             colorAttachments,
                               //!TODO const Array_Unsafe<Attachment_Reference>&    resolveAttachments,
                               const core::Optional<VkAttachmentReference*>&        depthStencilAttachment,
                               const core::span<core::uint32>&                      preserveAttachments);
};

}