#include <graphics/vulkan/pipeline.hpp>
#include <graphics/vulkan/device.hpp>
#include <graphics/vulkan/pipeline_layout.hpp>
#include <graphics/vulkan/render_pass.hpp>
#include <core/std_containers.hpp>

namespace crude_engine
{

Pipeline::Pipeline(Shared_Ptr<const Device>                           device,
                   Shared_Ptr<const Render_Pass>                      renderPass,
                   Shared_Ptr<const Pipeline_Layout>                  pipelineLayout,
                   Shared_Ptr<const Pipeline>                         basePipeline,
                   const Array_Unsafe<Shader_Stage_Create_Info>&      shaderStages,
                   const Optional<Vertex_Input_State_Create_Info>&    vertexInputState,
                   const Optional<Tessellation_State_Create_Info>&    tessellationState,
                   const Optional<Input_Assembly_State_Create_Info>&  inputAsseblyState,
                   const Optional<Viewport_State_Create_Info>&        viewportState,
                   const Optional<Rasterization_State_Create_Info>&   rasterizationState,
                   const Optional<Multisample_State_Create_Info>&     multisampleState,
                   const Optional<Depth_Stencil_State_Create_Info>&   depthStencilState,
                   const Optional<Color_Blend_State_Create_Info>&     colorBlendState,
                   const Optional<Dynamic_State_Create_Info>&         dynamicState,
                   uint32                                             subpass)
  :
  m_bindPoint(VK_PIPELINE_BIND_POINT_GRAPHICS),
  m_device(device),
  m_pipelineLayout(pipelineLayout),
  m_renderPass(renderPass),
  m_basePipeline(basePipeline)
{
  vector<VkPipelineShaderStageCreateInfo> vkShaderStages(shaderStages.size());
  Algorithms::copy(shaderStages.begin(), shaderStages.end(), vkShaderStages.begin());

  VkGraphicsPipelineCreateInfo vkCreateInfo{};
  vkCreateInfo.sType                = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  vkCreateInfo.pNext                = nullptr;
  vkCreateInfo.flags                = 0u;

  vkCreateInfo.stageCount           = static_cast<uint32>(vkShaderStages.size());
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
  vkCreateInfo.layout               = m_pipelineLayout ? CRUDE_OBJECT_HANDLE(m_pipelineLayout) : VK_NULL_HANDLE;
  vkCreateInfo.renderPass           = m_renderPass ? CRUDE_OBJECT_HANDLE(m_renderPass) : VK_NULL_HANDLE;
  vkCreateInfo.subpass              = subpass;
  vkCreateInfo.basePipelineHandle   = m_basePipeline ? CRUDE_OBJECT_HANDLE(m_basePipeline) : VK_NULL_HANDLE;
  vkCreateInfo.basePipelineIndex    = -1;

  VkResult result = vkCreateGraphicsPipelines(CRUDE_OBJECT_HANDLE(m_device), VK_NULL_HANDLE, 1u, &vkCreateInfo, getPVkAllocationCallbacks(), &m_handle);
  CRUDE_VULKAN_HANDLE_RESULT(result, "failed to create graphics pipeline");
}

Pipeline::~Pipeline()
{
  vkDestroyPipeline(CRUDE_OBJECT_HANDLE(m_device), m_handle, getPVkAllocationCallbacks());
}

const VkPipelineBindPoint Pipeline::getBindPoint() const
{
  return m_bindPoint;
}

Shared_Ptr<const Pipeline_Layout> Pipeline::getPipelineLayout()
{
  return m_pipelineLayout;
}

}