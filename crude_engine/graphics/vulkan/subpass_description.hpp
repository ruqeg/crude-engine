#pragma once

#include <core/array_unsafe.hpp>
#include <core/optional.hpp>
#include <graphics/vulkan/include_vulkan.hpp>

namespace crude_engine
{

class Device;

// ! sizeof(Subpass_Description) == sizeof(VkSubpassDescription)
// for render_pass.cpp
struct Subpass_Description final : public VkSubpassDescription
{
  explicit Subpass_Description(VkPipelineBindPoint                           pipelineBindPoint,
                               const Array_Unsafe<VkAttachmentReference>&    inputAttachments,
                               const Array_Unsafe<VkAttachmentReference>&    colorAttachments,
                               //!TODO const Array_Unsafe<Attachment_Reference>&  resolveAttachments,
                               const Optional<VkAttachmentReference*>&       depthStencilAttachment,
                               const Array_Unsafe<uint32>&                   preserveAttachments);
};

}