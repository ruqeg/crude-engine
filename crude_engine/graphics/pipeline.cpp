#include <vulkan/vulkan.hpp>

module crude.graphics.pipeline;

import crude.graphics.surface;
import crude.graphics.device;
import crude.graphics.vulkan_utils;
import crude.core.std_containers_heap;
import crude.core.algorithms;

namespace crude::graphics
{

Pipeline::Pipeline(core::shared_ptr<const Device>                           device,
                   core::shared_ptr<Render_Pass>                            renderPass,
                   core::shared_ptr<const Pipeline_Layout>                  pipelineLayout,
                   core::shared_ptr<const Pipeline>                         basePipeline,
                   const core::span<Shader_Stage_Create_Info>&              shaderStages,
                   const core::Optional<Vertex_Input_State_Create_Info>&    vertexInputState,
                   const core::Optional<Tessellation_State_Create_Info>&    tessellationState,
                   const core::Optional<Input_Assembly_State_Create_Info>&  inputAsseblyState,
                   const core::Optional<Viewport_State_Create_Info>&        viewportState,
                   const core::Optional<Rasterization_State_Create_Info>&   rasterizationState,
                   const core::Optional<Multisample_State_Create_Info>&     multisampleState,
                   const core::Optional<Depth_Stencil_State_Create_Info>&   depthStencilState,
                   const core::Optional<Color_Blend_State_Create_Info>&     colorBlendState,
                   const core::Optional<Dynamic_State_Create_Info>&         dynamicState,
                   core::uint32                                             subpass)
  :
  m_bindPoint(VK_PIPELINE_BIND_POINT_GRAPHICS),
  m_device(device),
  m_pipelineLayout(pipelineLayout),
  m_renderPass(renderPass),
  m_basePipeline(basePipeline)
{
  core::vector<VkPipelineShaderStageCreateInfo> vkShaderStages(shaderStages.size());
  core::Algorithms::copy(shaderStages.begin(), shaderStages.end(), vkShaderStages.begin());

  VkGraphicsPipelineCreateInfo vkCreateInfo{};
  vkCreateInfo.sType                = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  vkCreateInfo.pNext                = nullptr;
  vkCreateInfo.flags                = 0u;

  vkCreateInfo.stageCount           = static_cast<core::uint32>(vkShaderStages.size());
  vkCreateInfo.pStages              = vkShaderStages.data();
//==========
//!TODO fix unsafe
  vkCreateInfo.pVertexInputState    = vertexInputState.hasValue() ? &vertexInputState.value() : nullptr;
  vkCreateInfo.pTessellationState   = tessellationState.hasValue() ? &tessellationState.value() : nullptr;
  vkCreateInfo.pInputAssemblyState  = inputAsseblyState.hasValue() ? &inputAsseblyState.value() : nullptr;
  vkCreateInfo.pViewportState       = viewportState.hasValue() ? &viewportState.value() : nullptr;
  vkCreateInfo.pRasterizationState  = rasterizationState.hasValue() ? &rasterizationState.value() : nullptr;
  vkCreateInfo.pMultisampleState    = multisampleState.hasValue() ? &multisampleState.value() : nullptr;
  vkCreateInfo.pDepthStencilState   = depthStencilState.hasValue() ? &depthStencilState.value() : nullptr;
  vkCreateInfo.pColorBlendState     = colorBlendState.hasValue() ? &colorBlendState.value() : nullptr;
  vkCreateInfo.pDynamicState        = dynamicState.hasValue() ? &dynamicState.value() : nullptr;
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