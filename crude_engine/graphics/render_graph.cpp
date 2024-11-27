#include <vulkan/vulkan.hpp>

module crude.gfx.render_graph;

import crude.gfx.vk.render_pass;
import crude.gfx.vk.device;

namespace crude::gfx
{

Render_Graph_Pass::Render_Graph_Pass(Render_Graph& graph, core::uint32 index, VkPipelineStageFlags queue)
  : m_graph{ graph }
  , m_index{ index }
  , m_queue{ queue }
{}


void Render_Graph_Pass::setName(const core::string& name)
{
  m_name = name;
}

void Render_Graph_Pass::addColorOutput(const core::string& name, const Attachment_Info& attachmentInfo)
{
  auto resource = m_graph.getTextureResource(name);
  resource->setAttachmentInfo(attachmentInfo);
  m_colorOutputs.push_back(resource);
}

void Render_Graph_Pass::setDepthStencilOutput(const core::string& name, const Attachment_Info& attachmentInfo)
{
  auto resource = m_graph.getTextureResource(name);
  resource->setAttachmentInfo(attachmentInfo);
  m_depthOutput = resource;
}

void Render_Graph_Pass::addAttachmentInput(const core::string& name, const Attachment_Info& attachmentInfo)
{

}

void Render_Graph_Pass::addTextureInput(const core::string& name, const Attachment_Info& attachmentInfo)
{

}

void Render_Graph_Pass::build()
{
  core::vector<vk::Attachment_Description> attachmentsDescriptions;
  core::vector<VkImageLayout> colorLayouts;

  attachmentsDescriptions.reserve(m_colorOutputs.size());
  colorLayouts.reserve(m_colorOutputs.size());

  for (auto colorOutput : m_colorOutputs)
  {
    attachmentsDescriptions.push_back(vk::Attachment_Description({
      .format        = colorOutput->getAttachmentInfo().format,
      .samples       = colorOutput->getAttachmentInfo().samples,
      .colorOp       = vk::attachment_op::gDontCareStore,
      .stenicilOp    = vk::attachment_op::gClearStore,
      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      .finalLayout   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL }));
    colorLayouts.push_back(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
  }

  if (m_depthOutput)
  {
    attachmentsDescriptions.push_back(vk::Attachment_Description({
      .format        = m_depthOutput->getAttachmentInfo().format,
      .samples       = m_depthOutput->getAttachmentInfo().samples,
      .colorOp       = vk::attachment_op::gDontCareStore,
      .stenicilOp    = vk::attachment_op::gClearStore,
      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      .finalLayout   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL }));
  }

  vk::Subpass_Description subpassDescription;
  if (m_depthOutput)
  {
    subpassDescription = vk::Subpass_Description{ vk::Subpass_Description::Initialize_Color_Array_Depth { colorLayouts, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL } };
  }
  else
  {
    subpassDescription = vk::Subpass_Description{ vk::Subpass_Description::Initialize_Color_Array { colorLayouts } };
  }

  vk::Subpass_Dependency subpassDependency({
    .srcSubpass      = VK_SUBPASS_EXTERNAL,
    .dstSubpass      = 0,
    .srcStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
    .dstStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
    .srcAccessMask   = 0u,
    .dstAccessMask   = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
    .dependencyFlags = 0 });

  m_renderPass = core::allocateShared<vk::Render_Pass>(
    m_graph.m_device, 
    core::span<const vk::Subpass_Description>{ &subpassDescription, 1u },
    core::span<const vk::Subpass_Dependency>{ &subpassDependency, 1u },
    attachmentsDescriptions);
}

Render_Graph::Render_Graph(core::shared_ptr<vk::Device> device)
  : m_device{ device }
{}

core::shared_ptr<Render_Graph_Pass> Render_Graph::addPass(const core::string& name, VkPipelineStageFlags queue)
{
  if (m_passToIndex.contains(name))
  {
    return m_passes[m_passToIndex.find(name)->second];
  }

  core::uint32 index = m_passes.size();
  Render_Graph_Pass s(*this, index, queue);
  m_passes.push_back(core::allocateShared<Render_Graph_Pass>(s));
  m_passes.back()->setName(name);
  m_passToIndex[name] = index;
  return m_passes.back();
}

core::shared_ptr<Render_Texture_Resource> Render_Graph::getTextureResource(const core::string& name)
{
  if (m_resourceToIndex.contains(name))
  {
    return m_resources[m_resourceToIndex[name]];
  }

  core::uint32 index = m_resources.size();
  m_resources.push_back(core::allocateShared<Render_Texture_Resource>());
  m_resourceToIndex[name] = index;
  return m_resources.back();
}

}