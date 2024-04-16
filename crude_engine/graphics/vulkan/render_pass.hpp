#pragma once

#include <core/array_unsafe.hpp>
#include <core/shared_ptr.hpp>
#include <graphics/vulkan/subpass_dependency.hpp>
#include <graphics/vulkan/subpass_description.hpp>
#include <graphics/vulkan/attachment_description.hpp>
#include <graphics/vulkan/object.hpp>

namespace crude_engine
{

class Device;

class Render_Pass : public TObject<VkRenderPass>
{
public:
  explicit Render_Pass(Shared_Ptr<const Device>                     device,
                       const Array_Unsafe<Subpass_Description>&     subpasses,
                       const Array_Unsafe<Subpass_Dependency>&      subpassesDependencies,
                       const Array_Unsafe<Attachment_Description>&  attachments);
  ~Render_Pass();
private:
  Shared_Ptr<const Device>  m_device;
};

}
