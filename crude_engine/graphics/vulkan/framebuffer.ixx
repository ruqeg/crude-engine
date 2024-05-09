module;

#include <vulkan/vulkan.hpp>

export module crude_engine.graphics.vulkan.framebuffer;

import crude_engine.core.shared_ptr;
import crude_engine.core.std_containers_heap;
import crude_engine.graphics.vulkan.vulkan_object;

export namespace crude_engine
{

class Device;
class Render_Pass;
class Image_View;

class Framebuffer : public Vulkan_Object<VkFramebuffer>
{
public:
  explicit Framebuffer(Shared_Ptr<const Device>               device,
                       Shared_Ptr<Render_Pass>                renderPass,
                       const vector<Shared_Ptr<Image_View>>&  attachments,
                       uint32                                 width,
                       uint32                                 height,
                       uint32                                 layers);
  ~Framebuffer();
protected:
  Shared_Ptr<const Device>        m_device;
  Shared_Ptr<Render_Pass>         m_renderPass;
  vector<Shared_Ptr<Image_View>>  m_attachments;
};

}