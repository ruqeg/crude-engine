module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.render_pass;

import crude.core.std_containers_heap;
import crude.core.std_containers_stack;
import crude.graphics.vulkan_object;
export import crude.graphics.subpass_dependency;
export import crude.graphics.subpass_description;
export import crude.graphics.attachment_description;

export namespace crude::graphics
{

class Device;
class Image_View;

class Render_Pass : public Vulkan_Object<VkRenderPass>
{
public:
  explicit Render_Pass(core::shared_ptr<const Device>        device,
                       core::span<const Subpass_Description> subpasses,
                       core::span<const Subpass_Dependency>  subpassesDependencies,
                       core::vector<Attachment_Description>  attachmentsDescriptions);
  ~Render_Pass();
private:
  core::shared_ptr<const Device>        m_device;
  core::vector<Attachment_Description>  m_attachmentsDesc;
  friend class Command_Buffer;
};

}
