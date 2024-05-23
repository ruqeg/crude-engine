module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.framebuffer;

import crude.core.shared_ptr;
import crude.core.std_containers_heap;
import crude.graphics.vulkan_object;

export namespace crude::graphics
{

class Device;
class Render_Pass;
class Image_View;

class Framebuffer : public Vulkan_Object<VkFramebuffer>
{
public:
  explicit Framebuffer(core::Shared_Ptr<const Device>                     device,
                       core::Shared_Ptr<Render_Pass>                      renderPass,
                       const core::vector<core::Shared_Ptr<Image_View>>&  attachments,
                       core::uint32                                       width,
                       core::uint32                                       height,
                       core::uint32                                       layers);
  ~Framebuffer();
protected:
  core::Shared_Ptr<const Device>              m_device;
  core::Shared_Ptr<Render_Pass>               m_renderPass;
  core::vector<core::Shared_Ptr<Image_View>>  m_attachments;
};

}