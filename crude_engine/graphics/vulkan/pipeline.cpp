#include <vulkan/vulkan.hpp>

module crude.gfx.vk.pipeline;

import crude.gfx.vk.surface;
import crude.gfx.vk.device;
import crude.gfx.vk.vulkan_utils;
import crude.core.std_containers_heap;
import crude.core.algorithms;

namespace crude::gfx::vk
{

Pipeline::Pipeline(core::shared_ptr<const Device>                           device,
                   core::shared_ptr<Render_Pass>                            renderPass,
                   core::shared_ptr<const Pipeline_Layout>                  pipelineLayout,
                   core::shared_ptr<const Pipeline>                         basePipeline,
                   const core::span<Shader_Stage_Create_Info>&              shaderStages,
                   const core::optional<Vertex_Input_State_Create_Info>&    vertexInputState,
                   const core::optional<Tessellation_State_Create_Info>&    tessellationState,
                   const core::optional<Input_Assembly_State_Create_Info>&  inputAsseblyState,
                   const core::optional<Viewport_State_Create_Info>&        viewportState,
                   const core::optional<Rasterization_State_Create_Info>&   rasterizationState,
                   const core::optional<Multisample_State_Create_Info>&     multisampleState,
                   const core::optional<Depth_Stencil_State_Create_Info>&   depthStencilState,
                   const core::optional<Color_Blend_State_Create_Info>&     colorBlendState,
                   const core::optional<Dynamic_State_Create_Info>&         dynamicState,
                   core::uint32                                             subpass)
  :
  m_bindPoint(VK_PIPELINE_BIND_POINT_GRAPHICS),
  m_device(device),
  m_pipelineLayout(pipelineLayout),
  m_renderPass(renderPass),
  m_basePipeline(basePipeline)
{
  core::vector<VkPipelineShaderStageCreateInfo> vkShaderStages(shaderStages.size());
  core::copy(shaderStages.begin(), shaderStages.end(), vkShaderStages.begin());

  VkGraphicsPipelineCreateInfo vkCreateInfo{};
  vkCreateInfo.sType                = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  vkCreateInfo.pNext                = nullptr;
  vkCreateInfo.flags                = 0u;

  vkCreateInfo.stageCount           = static_cast<core::uint32>(vkShaderStages.size());
  vkCreateInfo.pStages              = vkShaderStages.data();
//==========
//!TODO fix unsafe
  vkCreateInfo.pVertexInputState    = vertexInputState.has_value() ? &vertexInputState.value() : nullptr;
  vkCreateInfo.pTessellationState   = tessellationState.has_value() ? &tessellationState.value() : nullptr;
  vkCreateInfo.pInputAssemblyState  = inputAsseblyState.has_value() ? &inputAsseblyState.value() : nullptr;
  vkCreateInfo.pViewportState       = viewportState.has_value() ? &viewportState.value() : nullptr;
  vkCreateInfo.pRasterizationState  = rasterizationState.has_value() ? &rasterizationState.value() : nullptr;
  vkCreateInfo.pMultisampleState    = multisampleState.has_value() ? &multisampleState.value() : nullptr;
  vkCreateInfo.pDepthStencilState   = depthStencilState.has_value() ? &depthStencilState.value() : nullptr;
  vkCreateInfo.pColorBlendState     = colorBlendState.has_value() ? &colorBlendState.value() : nullptr;
  vkCreateInfo.pDynamicState        = dynamicState.has_value() ? &dynamicState.value() : nullptr;
  //==========
  vkCreateInfo.layout               = m_pipelineLayout ? m_pipelineLayout->getHandle() : VK_NULL_HANDLE;
  vkCreateInfo.renderPass           = m_renderPass ? m_renderPass->getHandle() : VK_NULL_HANDLE;
  vkCreateInfo.subpass              = subpass;
  vkCreateInfo.basePipelineHandle   = m_basePipeline ? m_basePipeline->getHandle() : VK_NULL_HANDLE;
  vkCreateInfo.basePipelineIndex    = -1;

  VkResult result = vkCreateGraphicsPipelines(m_device->getHandle(), VK_NULL_HANDLE, 1u, &vkCreateInfo, getPVkAllocationCallbacks(), &m_handle);
  vulkanHandleResult(result, "failed to create graphics pipeline");
}

Pipeline::~Pipeline()
{
  vkDestroyPipeline(m_device->getHandle(), m_handle, getPVkAllocationCallbacks());
}

}