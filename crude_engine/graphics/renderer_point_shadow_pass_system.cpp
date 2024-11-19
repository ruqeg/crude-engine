#include <vulkan/vulkan.hpp>
#include <flecs.h>
#include <cstring>
#include <algorithm>
#include <DirectXMath.h>
#include <crude_shaders/point_shadow.mesh.inl>
#include <crude_shaders/point_shadow.task.inl>
#include <functional>

module crude.graphics.renderer_point_shadow_pass_system;

import crude.core.logger;
import crude.scene.image;
import crude.graphics.format_helper;
import crude.graphics.generate_mipmaps;
import crude.graphics.flush;
import crude.graphics.mesh_buffer;
import crude.scene.mesh;
import crude.graphics.render_pass;
import crude.graphics.command_buffer;
import crude.graphics.framebuffer;
import crude.graphics.pipeline;
import crude.graphics.swap_chain;
import crude.graphics.descriptor_pool;
import crude.graphics.descriptor_set_layout;
import crude.graphics.semaphore;
import crude.graphics.fence;
import crude.platform.sdl_window_container;
import crude.graphics.physical_device;
import crude.graphics.queue;
import crude.graphics.instance;
import crude.graphics.device;
import crude.graphics.surface;
import crude.graphics.swap_chain;
import crude.graphics.swap_chain_image;
import crude.graphics.image_cube;
import crude.graphics.image_view;
import crude.graphics.uniform_buffer;
import crude.graphics.material;
import crude.graphics.texture;
import crude.graphics.debug_utils_messenger;
import crude.graphics.command_pool;
import crude.graphics.image_attachment;
import crude.graphics.shader_module;
import crude.graphics.storage_buffer;
import crude.graphics.attachment_description;
import crude.graphics.gbuffer;

namespace crude::graphics
{

void rendererPointShadowPassSystemProcess(flecs::iter& it)
{
  Renderer_Point_Shadow_Pass_Systen_Ctx* pointShadowCtx = it.ctx<Renderer_Point_Shadow_Pass_Systen_Ctx>();
  core::shared_ptr<Renderer_Frame_System_Ctx> frameCtx = pointShadowCtx->frameCtx;
  core::shared_ptr<Renderer_Core_System_Ctx> coreCtx = frameCtx->coreCtx;

  if (pointShadowCtx->pointLightsShadowsCount < 1)
  {
    while (it.next());
    return;
  }

  core::array<VkClearValue, 1u> clearValues;
  clearValues[0].depthStencil = { 1.0f, 0 };

  VkRect2D renderArea;
  renderArea.extent = pointShadowCtx->pointShadowImageView->getImage()->getExtent2D();
  renderArea.offset = VkOffset2D{ 0, 0 };

  frameCtx->getFrameGraphicsCommandBuffer()->setViewport(Viewport({
    .x = 0.0f, .y = 0.0f,
    .width = static_cast<core::float32>(pointShadowCtx->pointShadowImageView->getImage()->getWidth()),
    .height = static_cast<core::float32>(pointShadowCtx->pointShadowImageView->getImage()->getHeight()),
    .minDepth = 0.0f, .maxDepth = 1.0f }));

  frameCtx->getFrameGraphicsCommandBuffer()->setScissor(Scissor({
    .offset = { 0, 0 },
    .extent = pointShadowCtx->pointShadowImageView->getImage()->getExtent2D() }));

  frameCtx->getFrameGraphicsCommandBuffer()->beginRenderPass(pointShadowCtx->pipeline->getRenderPass(), pointShadowCtx->framebuffers[frameCtx->swapchainImageIndex], clearValues, renderArea);
  frameCtx->getFrameGraphicsCommandBuffer()->bindPipeline(pointShadowCtx->pipeline);

  pointShadowCtx->perFrameBufferDescriptors[frameCtx->currentFrame].update(frameCtx->getFramePerFrameUniformBuffer());

  while (it.next())
  {
    auto meshBuffers = it.field<core::shared_ptr<Mesh_Buffer>>(0);
    auto meshes = it.field<core::shared_ptr<scene::Mesh>>(1);

    for (auto i : it)
    {
      pointShadowCtx->submeshesDrawsBufferDescriptor.update(meshBuffers[i]->getSubmeshesDrawsBuffer(), meshBuffers[i]->getSubmeshesDrawsBuffer()->getSize());
      pointShadowCtx->vertexBufferDescriptor.update(meshBuffers[i]->getVerticesBuffer(), meshBuffers[i]->getVerticesBuffer()->getSize());
      pointShadowCtx->meshletBufferDescriptor.update(meshBuffers[i]->getMeshletsBuffer(), meshBuffers[i]->getMeshletsBuffer()->getSize());
      pointShadowCtx->primitiveIndicesBufferDescriptor.update(meshBuffers[i]->getPrimitiveIndicesBuffer(), meshBuffers[i]->getPrimitiveIndicesBuffer()->getSize());
      pointShadowCtx->vertexIndicesBufferDescriptor.update(meshBuffers[i]->getVertexIndicesBuffer(), meshBuffers[i]->getVertexIndicesBuffer()->getSize());

      Per_Mesh perMesh;
      if (it.entity(i).has<scene::Transform>())
      {
        auto transform = it.entity(i).get_ref<scene::Transform>();
        perMesh.modelToWorld = transform->getNodeToWorldFloat4x4();
      }

      for (core::uint32 submeshIndex = 0u; submeshIndex < meshes[i]->submeshes.size(); ++submeshIndex)
      {
        const scene::Sub_Mesh& submesh = meshes[i]->submeshes[submeshIndex];

        core::array<VkWriteDescriptorSet, 7u> descriptorWrites;
        pointShadowCtx->perFrameBufferDescriptors[frameCtx->currentFrame].write(descriptorWrites[0]);
        pointShadowCtx->submeshesDrawsBufferDescriptor.write(descriptorWrites[1]);
        pointShadowCtx->vertexBufferDescriptor.write(descriptorWrites[2]);
        pointShadowCtx->meshletBufferDescriptor.write(descriptorWrites[3]);
        pointShadowCtx->primitiveIndicesBufferDescriptor.write(descriptorWrites[4]);   
        pointShadowCtx->vertexIndicesBufferDescriptor.write(descriptorWrites[5]);
        pointShadowCtx->pointLightsBufferDescriptor.write(descriptorWrites[6]);

        frameCtx->getFrameGraphicsCommandBuffer()->pushDescriptorSet(pointShadowCtx->pipeline, descriptorWrites);
        perMesh.submeshIndex = submeshIndex;
        frameCtx->getFrameGraphicsCommandBuffer()->pushConstant(pointShadowCtx->pipeline->getPipelineLayout(), perMesh);

        frameCtx->getFrameGraphicsCommandBuffer()->drawMeshTasks(6, pointShadowCtx->pointLightsShadowsCount);
      }
    }
  }

  frameCtx->getFrameGraphicsCommandBuffer()->endRenderPass();
}

Renderer_Point_Shadow_Pass_Systen_Ctx::Renderer_Point_Shadow_Pass_Systen_Ctx(core::shared_ptr<Renderer_Frame_System_Ctx> frameCtx)
  : perFrameBufferDescriptors{ cPerFrameUniformBufferDescriptor, cPerFrameUniformBufferDescriptor }
  , submeshesDrawsBufferDescriptor{ cSubmeshesDrawsBufferDescriptor }
  , vertexBufferDescriptor{ cVertexBufferDescriptor }
  , meshletBufferDescriptor{ cMeshletBufferDescriptor }
  , primitiveIndicesBufferDescriptor{ cPrimitiveIndicesBufferDescriptor }
  , vertexIndicesBufferDescriptor{ cVertexIndicesBufferDescriptor }
  , pointLightsBufferDescriptor{ cPointLightsBufferDescriptor }
  , frameCtx{ frameCtx }
  , dimensionSize{ 1024 }
  , samples{ VK_SAMPLE_COUNT_1_BIT }
  , depthStencilFormat{ findDepthFormatSupportedByDevice(frameCtx->coreCtx->device->getPhysicalDevice(), depth_formats::gDepthCandidates) }
{
  this->pointShadowImageView = core::allocateShared<Image_View>(core::allocateShared<Depth_Stencil_Cube_Attachment>(Depth_Stencil_Cube_Attachment::Initialize{
      .device             = frameCtx->coreCtx->device,
      .depthStencilFormat = this->depthStencilFormat,
      .dimension          = this->dimensionSize,
      .sampled            = true,
      .explicitResolve    = false,
      .mipLevelsCount     = 1u,
      .arrayCount         = 1u, 
      .samples            = this->samples }));

  initializeRenderPass();
  initalizeGraphicsPipeline();
}

void Renderer_Point_Shadow_Pass_Systen_Ctx::update(const core::vector<scene::Point_Light_GPU>& visiblePointLightsGPU)
{
  struct Point_Light_Shadow
  {
    DirectX::XMFLOAT4X4 worldToClip[6];
  };

  this->pointShadowImageView = core::allocateShared<Image_View>(core::allocateShared<Depth_Stencil_Cube_Attachment>(Depth_Stencil_Cube_Attachment::Initialize{
      .device             = frameCtx->coreCtx->device,
      .depthStencilFormat = this->depthStencilFormat,
      .dimension          = this->dimensionSize,
      .sampled            = true,
      .explicitResolve    = false,
      .mipLevelsCount     = 1u,
      .arrayCount         = (core::uint32)visiblePointLightsGPU.size(),
      .samples            = this->samples }));

  initializeFramebuffers();

  core::vector<Point_Light_Shadow> pointLightsShadows;
  pointLightsShadows.reserve(visiblePointLightsGPU.size());

  for (auto& visiblePointLightGPU : visiblePointLightsGPU)
  {
    Point_Light_Shadow pointLightShadow{};

    const DirectX::XMVECTOR position = DirectX::XMLoadFloat3(&visiblePointLightGPU.position);
    const std::array<DirectX::XMMATRIX, 6> worldsToLightView =
    {
      DirectX::XMMatrixLookAtLH(position, DirectX::XMVectorAdd(position, DirectX::XMVECTOR{ 1.f, 0.f, 0.f}), { 0.f, 1.f, 0.f}),
      DirectX::XMMatrixLookAtLH(position, DirectX::XMVectorAdd(position, DirectX::XMVECTOR{-1.f, 0.f, 0.f}), { 0.f, 1.f, 0.f}),
      DirectX::XMMatrixLookAtLH(position, DirectX::XMVectorAdd(position, DirectX::XMVECTOR{ 0.f, 1.f, 0.f}), { 0.f, 0.f,-1.f}),
      DirectX::XMMatrixLookAtLH(position, DirectX::XMVectorAdd(position, DirectX::XMVECTOR{ 0.f,-1.f, 0.f}), { 0.f, 0.f, 1.f}),
      DirectX::XMMatrixLookAtLH(position, DirectX::XMVectorAdd(position, DirectX::XMVECTOR{ 0.f, 0.f, 1.f}), { 0.f, 1.f, 0.f}),
      DirectX::XMMatrixLookAtLH(position, DirectX::XMVectorAdd(position, DirectX::XMVECTOR{ 0.f, 0.f,-1.f}), { 0.f, 1.f, 0.f})
    };

    const DirectX::XMMATRIX lightViewToClip = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, 1.0f, 0.01f, std::max(0.011f, visiblePointLightGPU.sourceRadius));

    for (int face = 0; face < 6; ++face)
    {
      DirectX::XMStoreFloat4x4(&pointLightShadow.worldToClip[face], DirectX::XMMatrixMultiply(worldsToLightView[face], lightViewToClip));
    }

    pointLightsShadows.push_back(pointLightShadow);
  }

  auto commandBuffer = core::allocateShared<graphics::Command_Buffer>(this->frameCtx->coreCtx->graphicsCommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
  this->pointLightsShadowsBuffer = core::allocateShared<graphics::Storage_Buffer>(commandBuffer, pointLightsShadows);

  if (this->pointLightsShadowsBuffer)
    this->pointLightsBufferDescriptor.update(this->pointLightsShadowsBuffer, this->pointLightsShadowsBuffer->getSize());
  else
    this->pointLightsBufferDescriptor.clear();

  this->pointLightsShadowsCount = pointLightsShadows.size();
}


core::shared_ptr<Descriptor_Set_Layout> Renderer_Point_Shadow_Pass_Systen_Ctx::createDescriptorSetLayout()
{
  core::shared_ptr<Renderer_Core_System_Ctx> coreCtx = frameCtx->coreCtx;
  auto descriptorPool = core::allocateShared<Descriptor_Pool>(coreCtx->device, getDescriptorPoolSizes(), 2);
  auto descriptorSetLayout = core::allocateShared<Descriptor_Set_Layout>(coreCtx->device, getDescriptorLayoutBindings(), true);
  return descriptorSetLayout;
}

void Renderer_Point_Shadow_Pass_Systen_Ctx::initializeRenderPass()
{
  core::shared_ptr<Renderer_Core_System_Ctx> coreCtx = frameCtx->coreCtx;

  core::array<Subpass_Dependency, 1u> subpassesDependencies =
  {
    Subpass_Dependency({
      .srcSubpass      = VK_SUBPASS_EXTERNAL,
      .dstSubpass      = 0,
      .srcStageMask    = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
      .dstStageMask    = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
      .srcAccessMask   = 0u,
      .dstAccessMask   = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
      .dependencyFlags = 0})
  };
  core::array<Subpass_Description, 1> subpassDescriptions
  {
    Subpass_Description(Subpass_Description::Initialize_Depth{
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL })
  };
  core::vector<Attachment_Description> attachmentsDescriptions =
  {
    Attachment_Description({
      .format        = this->depthStencilFormat,
      .samples       = this->samples,
      .colorOp       = attachment_op::gClearStore,
      .stenicilOp    = attachment_op::gClearStore,
      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      .finalLayout   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL })
  };

  renderPass = core::allocateShared<Render_Pass>(coreCtx->device, subpassDescriptions, subpassesDependencies, attachmentsDescriptions);
}

void Renderer_Point_Shadow_Pass_Systen_Ctx::initalizeGraphicsPipeline()
{
  core::shared_ptr<Renderer_Core_System_Ctx> coreCtx = frameCtx->coreCtx;
  core::shared_ptr<Shader_Module> meshShaderModule = core::allocateShared<Shader_Module>(coreCtx->device, crude::shaders::point_shadow::mesh);
  core::shared_ptr<Shader_Module> taskShaderModule = core::allocateShared<Shader_Module>(coreCtx->device, crude::shaders::point_shadow::task);

  core::array<Shader_Stage_Create_Info, 2u> shaderStagesInfo =
  {
    Mesh_Shader_Stage_Create_Info(meshShaderModule, "main"),
    Task_Shader_Stage_Create_Info(taskShaderModule, "main"),
  };

  Vertex_Input_State_Create_Info vertexInputStateInfo({
    .bindings = {},
    .attributes = {} });

  Tessellation_State_Create_Info tesselationInfo(3u);

  Input_Assembly_State_Create_Info inputAssembly({
    .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
    .primitiveRestartEnable = false });

  Viewport_State_Create_Info viewportState(1u, 1u);

  Rasterization_State_Create_Info rasterizer({
    .depthClampEnable = false,
    .rasterizerDiscardEnable = false,
    .polygonMode = VK_POLYGON_MODE_FILL,
    .cullMode = VK_CULL_MODE_BACK_BIT,
    .frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
    .depthBiasEnable = false,
    .depthBiasConstantFactor = 0.0f,
    .depthBiasClamp = 0.0f,
    .depthBiasSlopeFactor = 0.0f,
    .lineWidth = 1.f });

  Multisample_State_Create_Info multisampling({
    .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
    .sampleShadingEnable = VK_FALSE,
    .minSampleShading = 0.2f,
    .alphaToCoverageEnable = false,
    .alphaToOneEnable = false });

  Depth_Stencil_State_Create_Info depthStencilState({
    .depthTestEnable       = true,
    .depthWriteEnable      = true,
    .depthCompareOp        = VK_COMPARE_OP_LESS,
    .depthBoundsTestEnable = false,
    .stencilTestEnable     = false,
    .front                 = {},
    .back                  = {},
    .minDepthBounds        = 0.0f,
    .maxDepthBounds        = 1.0f });
  
  core::array<Pipeline_Color_Blend_Attachment_State, 1u> colorAttachments =
  {
    Pipeline_Color_Blend_Attachment_State({
      .blendEnable         = VK_TRUE,
      .colorWriteMask      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
      .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
      .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
      .colorBlendOp        = VK_BLEND_OP_ADD,
      .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
      .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
      .alphaBlendOp        = VK_BLEND_OP_ADD }),
  };
  Color_Blend_State_Create_Info colorBlendStateCreateInfo({
    .attachments    = colorAttachments,
    .blendConstants = { 0.f, 0.f, 0.f, 0.f },
    .logicOpEnable  = false,
    .logicOp        = VK_LOGIC_OP_COPY });

  core::array<VkDynamicState, 2> dynamicStates = {
    VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR
  };
  Dynamic_State_Create_Info dynamicStateInfo(dynamicStates);

  core::shared_ptr<Pipeline_Layout> pipelineLayout = core::allocateShared<Pipeline_Layout>(
    coreCtx->device, createDescriptorSetLayout(),
    Push_Constant_Range<Per_Mesh>(VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT));

  pipeline = core::allocateShared<Pipeline>(
    coreCtx->device,
    renderPass,
    pipelineLayout,
    nullptr,
    shaderStagesInfo,
    vertexInputStateInfo,
    tesselationInfo,
    inputAssembly,
    viewportState,
    rasterizer,
    multisampling,
    depthStencilState,
    colorBlendStateCreateInfo,
    dynamicStateInfo,
    0u);
}

void Renderer_Point_Shadow_Pass_Systen_Ctx::initializeFramebuffers()
{
  core::shared_ptr<Renderer_Core_System_Ctx> coreCtx = frameCtx->coreCtx;
  framebuffers.reserve(coreCtx->swapchainImages.size());
  for (core::uint32 i = 0; i < coreCtx->swapchainImages.size(); ++i)
  {
    framebuffers.push_back(core::allocateShared<Framebuffer>(
      coreCtx->device, renderPass, core::vector<core::shared_ptr<Image_View>>{ pointShadowImageView }, pointShadowImageView->getImage()->getExtent2D(), 6u));
  }
}

}