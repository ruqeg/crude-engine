module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.vulkan.render_pass;

import crude.core.shared_ptr;
import crude.core.std_containers_stack;
import crude.graphics.vulkan.vulkan_object;
import crude.graphics.vulkan.subpass_dependency;
import crude.graphics.vulkan.subpass_description;
import crude.graphics.vulkan.attachment_description;

export namespace crude::graphics
{

class Device;

class Render_Pass : public Vulkan_Object<VkRenderPass>
{
public:
  explicit Render_Pass(core::Shared_Ptr<const Device>             device,
                       const core::span<Subpass_Description>&     subpasses,
                       const core::span<Subpass_Dependency>&      subpassesDependencies,
                       const core::span<Attachment_Description>&  attachments);
  ~Render_Pass();
private:
  core::Shared_Ptr<const Device>  m_device;
};

}
