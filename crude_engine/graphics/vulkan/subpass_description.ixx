module;

#include <vulkan/vulkan.hpp>

export module crude_engine.graphics.vulkan.subpass_description;

import crude_engine.core.shared_ptr;
import crude_engine.core.optional;
import crude_engine.core.std_containers_stack;

export namespace crude_engine
{

class Device;

// ! sizeof(Subpass_Description) == sizeof(VkSubpassDescription)
// for render_pass.cpp
struct Subpass_Description final : public VkSubpassDescription
{
  explicit Subpass_Description(VkPipelineBindPoint                      pipelineBindPoint,
                               const span<VkAttachmentReference>&       inputAttachments,
                               const span<VkAttachmentReference>&       colorAttachments,
                               //!TODO const Array_Unsafe<Attachment_Reference>&  resolveAttachments,
                               const Optional<VkAttachmentReference*>&  depthStencilAttachment,
                               const span<uint32>&                      preserveAttachments);
};

}