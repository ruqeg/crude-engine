#include <vulkan/vulkan.hpp>

module crude.gfx.vk.graphics_pipeline;

import crude.gfx.vk.surface;
import crude.gfx.vk.device;
import crude.gfx.vk.vulkan_utils;
import crude.core.std_containers_heap;
import crude.core.algorithms;
import crude.gfx.vk.pipeline_layout;
import crude.gfx.vk.render_pass;
import crude.gfx.vk.utils;

namespace crude::gfx::vk
{

Graphics_Pipeline::Graphics_Pipeline(core::shared_ptr<Render_Pass> renderPass, core::shared_ptr<Pipeline_Layout> pipelineLayout, const Description& description)
  : Pipeline{ VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout }
  , m_renderPass{ renderPass }
  , m_basePipeline{ nullptr }
{
  core::vector<VkPipelineShaderStageCreateInfo> vkShaderStages(description.shaderStages.size());
  core::copy(description.shaderStages.begin(), description.shaderStages.end(), vkShaderStages.begin());

  VkGraphicsPipelineCreateInfo vkCreateInfo{};
  vkCreateInfo.sType                = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  vkCreateInfo.pNext                = nullptr;
  vkCreateInfo.flags                = 0u;
  vkCreateInfo.stageCount           = static_cast<core::uint32>(vkShaderStages.size());
  vkCreateInfo.pStages              = vkShaderStages.data();
  vkCreateInfo.layout               = getObjectHandle<VkPipelineLayout>(m_pipelineLayout);
  vkCreateInfo.renderPass           = getObjectHandle<VkRenderPass>(m_renderPass);
  vkCreateInfo.subpass              = 0u;
  vkCreateInfo.basePipelineHandle   = getObjectHandle<VkPipeline>(m_basePipeline);
  vkCreateInfo.basePipelineIndex    = -1;
  //==========
  //!TODO fix unsafe
#define _CRUDE_OPTIONAL_PTR_OR_NULL(val) (val.has_value() ? &val.value() : nullptr)
  vkCreateInfo.pVertexInputState = _CRUDE_OPTIONAL_PTR_OR_NULL(description.vertexInputState);
  vkCreateInfo.pTessellationState = _CRUDE_OPTIONAL_PTR_OR_NULL(description.tessellationState);
  vkCreateInfo.pInputAssemblyState = _CRUDE_OPTIONAL_PTR_OR_NULL(description.inputAsseblyState);
  vkCreateInfo.pViewportState = _CRUDE_OPTIONAL_PTR_OR_NULL(description.viewportState);
  vkCreateInfo.pRasterizationState = _CRUDE_OPTIONAL_PTR_OR_NULL(description.rasterizationState);
  vkCreateInfo.pMultisampleState = _CRUDE_OPTIONAL_PTR_OR_NULL(description.multisampleState);
  vkCreateInfo.pDepthStencilState = _CRUDE_OPTIONAL_PTR_OR_NULL(description.depthStencilState);
  vkCreateInfo.pColorBlendState = _CRUDE_OPTIONAL_PTR_OR_NULL(description.colorBlendState);
  vkCreateInfo.pDynamicState = _CRUDE_OPTIONAL_PTR_OR_NULL(description.dynamicState);
#undef _CRUDE_OPTIONAL_VALUE_PTR_OR_NULL
  //==========

  vulkanHandleResult(
    vkCreateGraphicsPipelines(m_device->getHandle(), VK_NULL_HANDLE, 1u, &vkCreateInfo, getPVkAllocationCallbacks(), &m_handle),
    "failed to create graphics pipeline");
}

core::shared_ptr<Render_Pass> Graphics_Pipeline::getRenderPass()
{
  return m_renderPass;
}

}