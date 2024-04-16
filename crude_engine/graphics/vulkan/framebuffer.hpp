#pragma once

#include <core/array_dynamic.hpp>
#include <core/shared_ptr.hpp>
#include <graphics/vulkan/include_vulkan.hpp>
#include <graphics/vulkan/object.hpp>

namespace crude_engine
{

class Device;
class Render_Pass;
class Image_View;

class Framebuffer : public TObject<VkFramebuffer>
{
public:
  explicit Framebuffer(Shared_Ptr<const Device>                      device,
                       Shared_Ptr<Render_Pass>                       renderPass,
                       const Array_Dynamic<Shared_Ptr<Image_View>>&  attachments,
                       uint32                                        width,
                       uint32                                        height,
                       uint32                                        layers);
  ~Framebuffer();
protected:
  Shared_Ptr<const Device>             m_device;
  Shared_Ptr<Render_Pass>              m_renderPass;
  Array_Dynamic<Shared_Ptr<Image_View>>  m_attachments;
};

}