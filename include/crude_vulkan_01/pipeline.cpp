#include "pipeline.hpp"
#include "device.hpp"
#include "pipeline_layout.hpp"
#include "render_pass.hpp"

namespace crude_vulkan_01
{

Graphics_Pipeline_Create_Info::Graphics_Pipeline_Create_Info(std::shared_ptr<const Device>                           device,
                                                             std::shared_ptr<const Render_Pass>                      renderPass,
                                                             std::shared_ptr<const Pipeline_Layout>                  pipelineLayout,
                                                             std::shared_ptr<const Pipeline>                         basePipeline,
                                                             const std::vector<Shader_Stage_Create_Info>&            shaderStages,
                                                             const std::optional<Vertex_Input_State_Create_Info>&    vertexInputState,
                                                             const std::optional<Input_Assembly_State_Create_Info>&  inputAsseblyState,
                                                             const std::optional<Viewport_State_Create_Info>&        viewportState,
                                                             const std::optional<Rasterization_State_Create_Info>&   rasterizationState,
                                                             const std::optional<Multisample_State_Create_Info>&     multisampleState,
                                                             const std::optional<Depth_Stencil_State_Create_Info>&   depthStencilState,
                                                             const std::optional<Color_Blend_State_Create_Info>&     colorBlendState,
                                                             const std::optional<Dynamic_State_Create_Info>&         dynamicState,
                                                             uint32                                                  subpass)
  :
  device(device),
  renderPass(renderPass),
  pipelineLayout(pipelineLayout),
  basePipeline(basePipeline),
  shaderStages(shaderStages),
  vertexInputState(vertexInputState),
  inputAsseblyState(inputAsseblyState),
  viewportState(viewportState),
  rasterizationState(rasterizationState),
  multisampleState(multisampleState),
  depthStencilState(depthStencilState),
  colorBlendState(colorBlendState),
  dynamicState(dynamicState),
  subpass(subpass)
{}

Pipeline::Pipeline(const Graphics_Pipeline_Create_Info& createInfo)
{
  m_device          = createInfo.device;
  m_pipelineLayout  = createInfo.pipelineLayout;
  m_renderPass      = createInfo.renderPass;
  m_basePipeline    = createInfo.basePipeline;

  std::vector<VkPipelineShaderStageCreateInfo> vkShaderStages(createInfo.shaderStages.begin(), createInfo.shaderStages.end());

  VkGraphicsPipelineCreateInfo vkCreateInfo{};
  vkCreateInfo.sType                = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  vkCreateInfo.pNext                = nullptr;
  vkCreateInfo.flags                = 0u;

  vkCreateInfo.stageCount           = static_cast<uint32>(vkShaderStages.size());
  vkCreateInfo.pStages              = vkShaderStages.data();
  vkCreateInfo.pVertexInputState    = CRUDE_VULKAN_01_OPTIONAL_REFVALUE_OR_NULL(createInfo.vertexInputState);
  vkCreateInfo.pTessellationState   = nullptr; // !TODO
  vkCreateInfo.pViewportState       = CRUDE_VULKAN_01_OPTIONAL_REFVALUE_OR_NULL(createInfo.viewportState);
  vkCreateInfo.pRasterizationState  = CRUDE_VULKAN_01_OPTIONAL_REFVALUE_OR_NULL(createInfo.rasterizationState);
  vkCreateInfo.pMultisampleState    = CRUDE_VULKAN_01_OPTIONAL_REFVALUE_OR_NULL(createInfo.multisampleState);
  vkCreateInfo.pDepthStencilState   = CRUDE_VULKAN_01_OPTIONAL_REFVALUE_OR_NULL(createInfo.depthStencilState);
  vkCreateInfo.pColorBlendState     = CRUDE_VULKAN_01_OPTIONAL_REFVALUE_OR_NULL(createInfo.colorBlendState);
  vkCreateInfo.pDynamicState        = CRUDE_VULKAN_01_OPTIONAL_REFVALUE_OR_NULL(createInfo.dynamicState);
  vkCreateInfo.layout               = m_pipelineLayout ? CRUDE_VULKAN_01_HANDLE(m_pipelineLayout) : VK_NULL_HANDLE;
  vkCreateInfo.renderPass           = m_renderPass ? CRUDE_VULKAN_01_HANDLE(m_renderPass) : VK_NULL_HANDLE;
  vkCreateInfo.subpass              = createInfo.subpass;
  vkCreateInfo.basePipelineHandle   = m_basePipeline ? CRUDE_VULKAN_01_HANDLE(m_basePipeline) : VK_NULL_HANDLE;
  vkCreateInfo.basePipelineIndex    = -1;

  VkResult result = vkCreateGraphicsPipelines(CRUDE_VULKAN_01_HANDLE(m_device), VK_NULL_HANDLE, 1u, &vkCreateInfo, nullptr, &m_handle);
  CRUDE_VULKAN_01_HANDLE_RESULT(result, "failed to create graphics pipeline");
}

Pipeline::~Pipeline()
{
  vkDestroyPipeline(CRUDE_VULKAN_01_HANDLE(m_device), m_handle, nullptr);
}

}