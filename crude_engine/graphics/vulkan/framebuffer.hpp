#pragma once

#include "../../core/core.hpp"
#include "include_vulkan.hpp"
#include "object.hpp"
#include <vector>

namespace crude_engine
{

class Device;
class Render_Pass;
class Image_View;

struct Framebuffer_Create_Info
{
  std::shared_ptr<const Device>             device;
  std::shared_ptr<Render_Pass>              renderPass;
  std::vector<std::shared_ptr<Image_View>>  attachments;
  uint32                                    width;
  uint32                                    height;
  uint32                                    layers;
  explicit Framebuffer_Create_Info(std::shared_ptr<const Device>                    device,
                                   std::shared_ptr<Render_Pass>                     renderPass,
                                   const std::vector<std::shared_ptr<Image_View>>&  attachments,
                                   uint32                                           width,
                                   uint32                                           height,
                                   uint32                                           layers);
};

class Framebuffer : public TObject<VkFramebuffer>
{
public:
  explicit Framebuffer(const Framebuffer_Create_Info& createInfo);
  ~Framebuffer();
protected:
  std::shared_ptr<const Device>             m_device;
  std::shared_ptr<Render_Pass>              m_renderPass;
  std::vector<std::shared_ptr<Image_View>>  m_attachments;
};

}