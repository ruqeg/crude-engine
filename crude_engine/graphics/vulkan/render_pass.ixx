module;

#include <vulkan/vulkan.hpp>

export module crude_engine.graphics.vulkan.render_pass;

import crude_engine.core.shared_ptr;
import crude_engine.core.std_containers_stack;
import crude_engine.graphics.vulkan.vulkan_object;
import crude_engine.graphics.vulkan.subpass_dependency;
import crude_engine.graphics.vulkan.subpass_description;
import crude_engine.graphics.vulkan.attachment_description;

export namespace crude_engine
{

class Device;

class Render_Pass : public Vulkan_Object<VkRenderPass>
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
