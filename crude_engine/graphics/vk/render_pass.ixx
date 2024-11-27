module;

#include <vulkan/vulkan.hpp>

export module crude.gfx.vk.render_pass;

import crude.core.std_containers_heap;
import crude.core.std_containers_stack;
import crude.gfx.vk.vulkan_object;
export import crude.gfx.vk.subpass_dependency;
export import crude.gfx.vk.subpass_description;
export import crude.gfx.vk.attachment_description;

export namespace crude::gfx::vk
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
