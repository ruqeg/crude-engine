module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.gbuffer;

export import crude.core.std_containers_stack;
export import crude.core.std_containers_heap;

export namespace crude::graphics
{

class Depth_Stencil_Attachment;
class Color_Attachment;
class Device;
class Image_View;

class GBuffer
{
public:
  GBuffer(core::shared_ptr<const Device> device, const VkExtent2D& extent);
  core::shared_ptr<const Device> getDevice();
  core::shared_ptr<Color_Attachment> getAlbedoAttachment();
  core::shared_ptr<Depth_Stencil_Attachment> getDepthStencilAttachment();
  core::shared_ptr<Image_View> getAlbedoAttachmentView();
  core::shared_ptr<Image_View> getDepthStencilAttachmentView();
  const VkExtent2D& getExtent();
private:
  core::shared_ptr<Color_Attachment>          m_albedoAttachment;
  core::shared_ptr<Depth_Stencil_Attachment>  m_depthStencilAttachment;
  core::shared_ptr<Image_View>                m_albedoAttachmentView;
  core::shared_ptr<Image_View>                m_depthStencilAttachmentView;
  VkExtent2D                                  m_extent;
};

}