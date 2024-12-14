/*#include <vulkan/vulkan.h>
#include <crude_shaders/deferred_gbuffer.frag.inl>
#include <crude_shaders/deferred_gbuffer.mesh.inl>
#include <crude_shaders/deferred_gbuffer.task.inl>

module crude.gfx.deferred_render_pass_scene_renderer;

import crude.gfx.vk.device;
import crude.gfx.vk.pipeline
import crude.gfx.vk.buffer_descriptor;
import crude.gfx.vk.image_descriptor;

namespace crude::gfx
{

Deferred_Scene_Renderer::Deferred_Scene_Renderer(core::shared_ptr<vk::Device> device, core::shared_ptr<vk::Render_Pass> renderPass)
  : m_device{ device }
  , m_renderPass{ renderPass }
  , m_descriptorLayoutBindings { 
    vk::Uniform_Buffer_Descriptor{ 0u, VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_FRAGMENT_BIT },
    vk::Storage_Buffer_Descriptor{ 1u, VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT },
    vk::Storage_Buffer_Descriptor{ 2u, VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT },
    vk::Storage_Buffer_Descriptor{ 3u, VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT },
    vk::Storage_Buffer_Descriptor{ 4u, VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT },
    vk::Storage_Buffer_Descriptor{ 5u, VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT },
    vk::Storage_Buffer_Descriptor{ 6u, VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT },
    vk::Combined_Image_Sampler_Descriptor{ 7u, VK_SHADER_STAGE_FRAGMENT_BIT },
    vk::Combined_Image_Sampler_Descriptor{ 8u, VK_SHADER_STAGE_FRAGMENT_BIT },
    vk::Combined_Image_Sampler_Descriptor{ 9u, VK_SHADER_STAGE_FRAGMENT_BIT } }
{
  // !TODO
  struct Per_Mesh
  {
    DirectX::XMFLOAT4X4  modelToWorld;
    core::uint32         submeshIndex;
  };
  core::shared_ptr<vk::Descriptor_Set_Layout> descriptorSetLayout = core::allocateShared<vk::Descriptor_Set_Layout>(m_device, m_descriptorLayoutBindings, true);
  core::shared_ptr<vk::Pipeline_Layout> pipelineLayout = core::allocateShared<vk::Pipeline_Layout>(
    m_device, 
    descriptorSetLayout,
    vk::Push_Constant_Range<Per_Mesh>(VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT));

  core::shared_ptr<vk::Shader_Module> taskShaderModule = core::allocateShared<vk::Shader_Module>(m_device, crude::shaders::deferred_gbuffer::task);
  core::shared_ptr<vk::Shader_Module> meshShaderModule = core::allocateShared<vk::Shader_Module>(m_device, crude::shaders::deferred_gbuffer::mesh);
  core::shared_ptr<vk::Shader_Module> fragShaderModule = core::allocateShared<vk::Shader_Module>(m_device, crude::shaders::deferred_gbuffer::frag);

  core::array<vk::Shader_Stage_Create_Info, 3u> shaderStagesInfo =
  {
    vk::Task_Shader_Stage_Create_Info(taskShaderModule, "main"),
    vk::Mesh_Shader_Stage_Create_Info(meshShaderModule, "main"),
    vk::Fragment_Shader_Stage_Create_Info(fragShaderModule, "main"),
  };

  vk::Vertex_Input_State_Create_Info vertexInputStateInfo({
    .bindings   = {},
    .attributes = {} });

  vk::Tessellation_State_Create_Info tesselationInfo(3u);

  vk::Input_Assembly_State_Create_Info inputAssembly({
    .topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
    .primitiveRestartEnable = false });

  vk::Viewport_State_Create_Info viewportState(1u, 1u);

  vk::Rasterization_State_Create_Info rasterizer({
    .depthClampEnable        = false,
    .rasterizerDiscardEnable = false,
    .polygonMode             = VK_POLYGON_MODE_FILL,
    .cullMode                = VK_CULL_MODE_BACK_BIT,
    .frontFace               = VK_FRONT_FACE_COUNTER_CLOCKWISE,
    .depthBiasEnable         = false,
    .depthBiasConstantFactor = 0.0f,
    .depthBiasClamp          = 0.0f,
    .depthBiasSlopeFactor    = 0.0f,
    .lineWidth               = 1.f });

  vk::Multisample_State_Create_Info multisampling({
    .rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT,
    .sampleShadingEnable   = VK_FALSE,
    .minSampleShading      = 0.2f,
    .alphaToCoverageEnable = false,
    .alphaToOneEnable      = false });

  core::array<VkDynamicState, 2> dynamicStates = {
    VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR
  };

  vk::Dynamic_State_Create_Info dynamicStateInfo(dynamicStates);

  core::array<vk::Pipeline_Color_Blend_Attachment_State, 3u> colorBlendAttachmentsStates =
  {
    vk::Pipeline_Color_Blend_Attachment_State({
      .blendEnable = VK_TRUE,
      .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
      .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
      .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
      .colorBlendOp = VK_BLEND_OP_ADD,
      .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
      .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
      .alphaBlendOp = VK_BLEND_OP_ADD }),
    vk::Pipeline_Color_Blend_Attachment_State({
      .blendEnable = VK_FALSE,
      .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT,
      .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
      .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
      .colorBlendOp = VK_BLEND_OP_ADD,
      .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
      .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
      .alphaBlendOp = VK_BLEND_OP_ADD }),
    vk::Pipeline_Color_Blend_Attachment_State({
      .blendEnable = VK_FALSE,
      .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
      .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
      .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
      .colorBlendOp = VK_BLEND_OP_ADD,
      .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
      .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
      .alphaBlendOp = VK_BLEND_OP_ADD })
  };
  vk::Color_Blend_State_Create_Info colorBlendStateInfo({
    .attachments = colorAttachments,
    .blendConstants = { 0.f, 0.f, 0.f, 0.f },
    .logicOpEnable = false,
    .logicOp = VK_LOGIC_OP_COPY });

  vk::Depth_Stencil_State_Create_Info depthStencilStateInfo({
    .depthTestEnable = true,
    .depthWriteEnable = true,
    .depthCompareOp = VK_COMPARE_OP_LESS,
    .depthBoundsTestEnable = false,
    .stencilTestEnable = false,
    .front = {},
    .back = {},
    .minDepthBounds = 0.0f,
    .maxDepthBounds = 1.0f });

  m_pipeline = core::allocateShared<vk::Pipeline>(
    m_device,
    m_renderPass,
    pipelineLayout,
    nullptr,
    shaderStagesInfo,
    vertexInputStateInfo,
    tesselationInfo,
    inputAssembly,
    viewportState,
    rasterizer,
    multisampling,
    depthStencilStateInfo,
    colorBlendStateInfo,
    dynamicStateInfo,
    0u);
}

void Deferred_Scene_Renderer::render()
{
}

}*/