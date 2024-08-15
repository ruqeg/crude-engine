module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.framebuffer;

import crude.core.std_containers_heap;
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
  explicit Framebuffer(core::shared_ptr<const Device>                     device,
                       core::shared_ptr<Render_Pass>                      renderPass,
                       const core::vector<core::shared_ptr<Image_View>>&  attachments,
                       const VkExtent2D&                                  extent,
                       core::uint32                                       layers);
  explicit Framebuffer(core::shared_ptr<const Device>                     device,
                       core::shared_ptr<Render_Pass>                      renderPass,
                       const core::vector<core::shared_ptr<Image_View>>&  attachments,
                       core::uint32                                       width,
                       core::uint32                                       height,
                       core::uint32                                       layers);
  ~Framebuffer();
protected:
  core::shared_ptr<const Device>              m_device;
  core::shared_ptr<Render_Pass>               m_renderPass;
  core::vector<core::shared_ptr<Image_View>>  m_attachments;
  friend class Command_Buffer;
};

}