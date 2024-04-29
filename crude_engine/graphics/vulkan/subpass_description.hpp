#pragma once

#include <core/std_containers.hpp>
#include <core/optional.hpp>
#include <graphics/vulkan/include_vulkan.hpp>

namespace crude_engine
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