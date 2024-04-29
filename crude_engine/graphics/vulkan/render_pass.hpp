#pragma once

#include <core/std_containers.hpp>
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
  explicit Render_Pass(Shared_Ptr<const Device>             device,
                       const span<Subpass_Description>&     subpasses,
                       const span<Subpass_Dependency>&      subpassesDependencies,
                       const span<Attachment_Description>&  attachments);
  ~Render_Pass();
private:
  Shared_Ptr<const Device>  m_device;
};

}
