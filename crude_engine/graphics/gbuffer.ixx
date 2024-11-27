module;

#include <vulkan/vulkan.hpp>

export module crude.gfx.gbuffer;

export import crude.core.std_containers_stack;
export import crude.core.std_containers_heap;

export namespace crude::gfx::vk
{

class Depth_Stencil_Attachment;
class Color_Attachment;
class Device;
class Image_View;

}

export namespace crude::gfx
{

class GBuffer
{
public:
  GBuffer(core::shared_ptr<const vk::Device> device, const VkExtent2D& extent);
  core::shared_ptr<const vk::Device> getDevice();
  core::shared_ptr<vk::Color_Attachment> getAlbedoAttachment();
  core::shared_ptr<vk::Image_View> getAlbedoAttachmentView();
  core::shared_ptr<vk::Color_Attachment> getMetallicRoughnessAttachment();
  core::shared_ptr<vk::Image_View> getMetallicRoughnessAttachmentView();
  core::shared_ptr<vk::Color_Attachment> getNormalAttachment();
  core::shared_ptr<vk::Image_View> getNormalAttachmentView();
  core::shared_ptr<vk::Depth_Stencil_Attachment> getDepthStencilAttachment();
  core::shared_ptr<vk::Image_View> getDepthStencilAttachmentView();
  const VkExtent2D& getExtent();
private:
  core::shared_ptr<vk::Color_Attachment>          m_albedoAttachment;
  core::shared_ptr<vk::Image_View>                m_albedoAttachmentView;
  core::shared_ptr<vk::Color_Attachment>          m_metallicRoughnessAttachment;
  core::shared_ptr<vk::Image_View>                m_metallicRoughnessAttachmentView;
  core::shared_ptr<vk::Color_Attachment>          m_normalAttachment;
  core::shared_ptr<vk::Image_View>                m_normalAttachmentView;
  core::shared_ptr<vk::Depth_Stencil_Attachment>  m_depthStencilAttachment;
  core::shared_ptr<vk::Image_View>                m_depthStencilAttachmentView;
  VkExtent2D                                      m_extent;
};

}