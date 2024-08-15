#include <vulkan/vulkan.hpp>

module crude.graphics.gbuffer;

import crude.graphics.attachment_description;
import crude.graphics.device;
import crude.graphics.physical_device;
import crude.graphics.format_helper;
import crude.graphics.image_view;
import crude.graphics.attachment_description;
import crude.graphics.image_attachment;

namespace crude::graphics
{

GBuffer::GBuffer(core::shared_ptr<const Device> device, const VkExtent2D& extent)
  : m_extent{extent}
{
  m_albedoAttachment = core::allocateShared<Color_Attachment>(Color_Attachment::Initialize{
    .device          = device,
    .format          = VK_FORMAT_B8G8R8A8_SRGB,
    .extent          = m_extent,
    .sampled         = true,
    .explicitResolve = false,
    .mipLevelsCount  = 1u,
    .samples         = device->getPhysicalDevice()->getProperties().getMaximumUsableSampleCount() });

  const VkFormat depthFormat = findDepthFormatSupportedByDevice(device->getPhysicalDevice(), depth_formats::gDepthCandidates);
  m_depthStencilAttachment = core::allocateShared<Depth_Stencil_Attachment>(Depth_Stencil_Attachment::Initialize{
    .device             = device,
    .depthStencilFormat = depthFormat,
    .extent             = m_extent,
    .sampled            = true,
    .explicitResolve    = false,
    .mipLevelsCount     = 1u,
    .samples            = device->getPhysicalDevice()->getProperties().getMaximumUsableSampleCount() });

  m_albedoAttachmentView = core::allocateShared<Image_View>(m_albedoAttachment);
  m_depthStencilAttachmentView = core::allocateShared<Image_View>(m_depthStencilAttachment);
}

core::shared_ptr<Color_Attachment> GBuffer::getAlbedoAttachment() { return m_albedoAttachment; }
core::shared_ptr<Depth_Stencil_Attachment> GBuffer::getDepthStencilAttachment() { return m_depthStencilAttachment; }
core::shared_ptr<Image_View> GBuffer::getAlbedoAttachmentView() { return m_albedoAttachmentView; }
core::shared_ptr<Image_View> GBuffer::getDepthStencilAttachmentView() { return m_depthStencilAttachmentView; }
const VkExtent2D& GBuffer::getExtent() { return m_extent; }
core::shared_ptr<const Device> GBuffer::getDevice() { return m_albedoAttachment->getDevice(); }

}