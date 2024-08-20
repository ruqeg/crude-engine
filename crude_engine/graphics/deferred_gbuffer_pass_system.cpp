#include <vulkan/vulkan.hpp>
#include <flecs.h>
#include <cstring>
#include <algorithm>
#include <DirectXMath.h>
#include <crude_shaders/deferred_gbuffer.frag.inl>
#include <crude_shaders/deferred_gbuffer.mesh.inl>
#include <crude_shaders/deferred_gbuffer.task.inl>
#include <functional>

module crude.graphics.deferred_gbuffer_pass_system;

import crude.core.logger;
import crude.scene.image;
import crude.graphics.format_helper;
import crude.graphics.generate_mipmaps;
import crude.graphics.flush;
import crude.graphics.mesh_buffer;
import crude.scene.mesh;
import crude.scene.free_camera_script;
import crude.resources.gltf_loader;
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
import crude.graphics.image_view;
import crude.graphics.uniform_buffer;
import crude.graphics.debug_utils_messenger;
import crude.graphics.image_view;
import crude.graphics.command_pool;
import crude.graphics.image_attachment;
import crude.graphics.shader_module;
import crude.graphics.renderer_core_component;
import crude.graphics.storage_buffer;
import crude.graphics.attachment_description;
import crude.graphics.gbuffer;

namespace crude::graphics
{

const Uniform_Buffer_Descriptor          cPerFrameUniformBufferDescriptor(0u, VK_SHADER_STAGE_MESH_BIT_EXT);
const Combined_Image_Sampler_Descriptor  cSubmeshTextureDescriptor(6u, VK_SHADER_STAGE_FRAGMENT_BIT);
const Storage_Buffer_Descriptor          cSubmeshesDrawsBufferDescriptor{ 1u, VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT };
const Storage_Buffer_Descriptor          cVertexBufferDescriptor{ 3u, VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT };
const Storage_Buffer_Descriptor          cMeshletBufferDescriptor{ 2u, VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT };
const Storage_Buffer_Descriptor          cPrimitiveIndicesBufferDescriptor{ 4u, VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT };
const Storage_Buffer_Descriptor          cVertexIndicesBufferDescriptor{ 5u, VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT };

core::array<Descriptor_Set_Layout_Binding, 7u> cDescriptorLayoutBindings = 
{
  cPerFrameUniformBufferDescriptor,
  cSubmeshTextureDescriptor,
  cSubmeshesDrawsBufferDescriptor,
  cVertexBufferDescriptor,
  cMeshletBufferDescriptor,
  cPrimitiveIndicesBufferDescriptor,
  cVertexIndicesBufferDescriptor
};

core::array<Descriptor_Pool_Size, 7u> cDescriptorPoolSizes
{
  Uniform_Buffer_Pool_Size(cFramesCount),
  Combined_Image_Sampler_Pool_Size(cFramesCount),
  Storage_Buffer_Pool_Size(1u),
  Storage_Buffer_Pool_Size(1u),
  Storage_Buffer_Pool_Size(1u),
  Storage_Buffer_Pool_Size(1u),
  Storage_Buffer_Pool_Size(1u)
};

struct Per_Mesh
{
  // !TODO Move
  DirectX::XMFLOAT4X4  modelToWorld;
  core::uint32         submeshIndex;
};

Deferred_GBuffer_Pass_Component::Deferred_GBuffer_Pass_Component()
  : perFrameBufferDescriptors{ cPerFrameUniformBufferDescriptor, cPerFrameUniformBufferDescriptor }
  , submeshTextureDescriptors{ cSubmeshTextureDescriptor, cSubmeshTextureDescriptor }
  , submeshesDrawsBufferDescriptor{ cSubmeshesDrawsBufferDescriptor }
  , vertexBufferDescriptor{ cVertexBufferDescriptor }
  , meshletBufferDescriptor{ cMeshletBufferDescriptor }
  , primitiveIndicesBufferDescriptor{ cPrimitiveIndicesBufferDescriptor }
  , vertexIndicesBufferDescriptor{ cVertexIndicesBufferDescriptor }
{}

Deferred_GBuffer_Pass_Component::Deferred_GBuffer_Pass_Component(core::shared_ptr<Device> device, const VkExtent2D& extent, core::uint32 swapchainImagesCount)
  : Deferred_GBuffer_Pass_Component()
{
  gbuffer = core::allocateShared<GBuffer>(device, extent);
  initializeRenderPass();
  initalizeGraphicsPipeline();
  initializeFramebuffers(swapchainImagesCount);
}

core::shared_ptr<Descriptor_Set_Layout> Deferred_GBuffer_Pass_Component::createDescriptorSetLayout()
{
  auto descriptorPool = core::allocateShared<Descriptor_Pool>(gbuffer->getDevice(), cDescriptorPoolSizes);
  auto descriptorSetLayout = core::allocateShared<Descriptor_Set_Layout>(gbuffer->getDevice(), cDescriptorLayoutBindings, true);
  return descriptorSetLayout;
}

void Deferred_GBuffer_Pass_Component::initializeRenderPass()
{
  core::array<Subpass_Dependency, 1u> subpassesDependencies =
  {
    Subpass_Dependency({
      .srcSubpass      = VK_SUBPASS_EXTERNAL,
      .dstSubpass      = 0,
      .srcStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
      .dstStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
      .srcAccessMask   = 0u,
      .dstAccessMask   = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
      .dependencyFlags = 0})
  };
  renderPass = core::allocateShared<Render_Pass>(gbuffer->getDevice(), getSubpassDescriptions(), subpassesDependencies, getAttachmentsDescriptions());
}

void Deferred_GBuffer_Pass_Component::initalizeGraphicsPipeline()
{
  core::shared_ptr<Shader_Module> taskShaderModule = core::allocateShared<Shader_Module>(gbuffer->getDevice(), crude::shaders::deferred_gbuffer::task);
  core::shared_ptr<Shader_Module> meshShaderModule = core::allocateShared<Shader_Module>(gbuffer->getDevice(), crude::shaders::deferred_gbuffer::mesh);
  core::shared_ptr<Shader_Module> fragShaderModule = core::allocateShared<Shader_Module>(gbuffer->getDevice(), crude::shaders::deferred_gbuffer::frag);

  core::array<Shader_Stage_Create_Info, 3u> shaderStagesInfo =
  {
    Task_Shader_Stage_Create_Info(taskShaderModule, "main"),
    Mesh_Shader_Stage_Create_Info(meshShaderModule, "main"),
    Fragment_Shader_Stage_Create_Info(fragShaderModule, "main"),
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

  core::array<VkDynamicState, 2> dynamicStates = {
    VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR
  };
  Dynamic_State_Create_Info dynamicStateInfo(dynamicStates);

  core::shared_ptr<Pipeline_Layout> pipelineLayout = core::allocateShared<Pipeline_Layout>(
    gbuffer->getDevice(), createDescriptorSetLayout(),
    Push_Constant_Range<Per_Mesh>(VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT));

  pipeline = core::allocateShared<Pipeline>(
    gbuffer->getDevice(),
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
    createDepthStencilStateCreateInfo(),
    createColorBlendStateCreateInfo(),
    dynamicStateInfo,
    0u);
}

void Deferred_GBuffer_Pass_Component::initializeFramebuffers(core::uint32 swapchainImagesCount)
{
  framebuffers.reserve(swapchainImagesCount);
  for (core::uint32 i = 0; i < swapchainImagesCount; ++i)
  {
    framebuffers.push_back(core::allocateShared<Framebuffer>(gbuffer->getDevice(), pipeline->getRenderPass(), getFramebufferAttachments(), gbuffer->getExtent(), 1u));
  }
}

core::array<Subpass_Description, 1> Deferred_GBuffer_Pass_Component::getSubpassDescriptions()
{
  return 
  {
    Subpass_Description(Subpass_Description::Initialize_Color_Array_Depth{
      core::array
      {
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
      },
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL })
  };
}

core::vector<Attachment_Description> Deferred_GBuffer_Pass_Component::getAttachmentsDescriptions()
{
  return 
  {
    Attachment_Description({
      .format        = gbuffer->getAlbedoAttachment()->getFormat(),
      .samples       = gbuffer->getAlbedoAttachment()->getSampleCount(),
      .colorOp       = attachment_op::gClearStore,
      .stenicilOp    = attachment_op::gClearStore,
      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      .finalLayout   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL }),
    Attachment_Description({
      .format        = gbuffer->getDepthStencilAttachment()->getFormat(),
      .samples       = gbuffer->getDepthStencilAttachment()->getSampleCount(),
      .colorOp       = attachment_op::gClearStore,
      .stenicilOp    = attachment_op::gClearStore,
      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      .finalLayout   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL })
  };
}

core::vector<core::shared_ptr<Image_View>> Deferred_GBuffer_Pass_Component::getFramebufferAttachments()
{
  return { gbuffer->getAlbedoAttachmentView(), gbuffer->getDepthStencilAttachmentView() };
}

Color_Blend_State_Create_Info Deferred_GBuffer_Pass_Component::createColorBlendStateCreateInfo()
{
  core::array<Pipeline_Color_Blend_Attachment_State, 1u> colorAttachments =
  {
    Pipeline_Color_Blend_Attachment_State({
      .blendEnable = VK_FALSE,
      .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
      .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
      .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
      .colorBlendOp = VK_BLEND_OP_ADD,
      .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
      .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
      .alphaBlendOp = VK_BLEND_OP_ADD })
  };
  return Color_Blend_State_Create_Info({
    .attachments = colorAttachments,
    .blendConstants = { 0.f, 0.f, 0.f, 0.f },
    .logicOpEnable = false,
    .logicOp = VK_LOGIC_OP_COPY });
}

Depth_Stencil_State_Create_Info Deferred_GBuffer_Pass_Component::createDepthStencilStateCreateInfo()
{
  return Depth_Stencil_State_Create_Info({
    .depthTestEnable = true,
    .depthWriteEnable = true,
    .depthCompareOp = VK_COMPARE_OP_LESS,
    .depthBoundsTestEnable = false,
    .stencilTestEnable = false,
    .front = {},
    .back = {},
    .minDepthBounds = 0.0f,
    .maxDepthBounds = 1.0f });
}

void deferredGBufferPassSystemProcess(flecs::iter& it)
{
  Deferred_GBuffer_Pass_Component* deferredGBufferComponent = it.world().get_mut<Deferred_GBuffer_Pass_Component>();
  Renderer_Frame_Component* frameComponent = it.world().get_mut<Renderer_Frame_Component>();
  Renderer_Core_Component* coreComponent = it.world().get_mut<Renderer_Core_Component>();
  
  core::array<VkClearValue, 2u> clearValues;;
  clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
  clearValues[1].depthStencil = { 1.0f, 0 };

  VkRect2D renderArea;
  renderArea.extent = coreComponent->swapchain->getExtent();
  renderArea.offset = VkOffset2D { 0, 0 };

  frameComponent->getFrameGraphicsCommandBuffer()->setViewport(Viewport({
    .x = 0.0f, .y = 0.0f,
    .width = static_cast<core::float32>(coreComponent->swapchain->getExtent().width),
    .height = static_cast<core::float32>(coreComponent->swapchain->getExtent().height),
    .minDepth = 0.0f, .maxDepth = 1.0f }));

  frameComponent->getFrameGraphicsCommandBuffer()->setScissor(Scissor({
    .offset = { 0, 0 },
    .extent = coreComponent->swapchain->getExtent() }));

  frameComponent->getFrameGraphicsCommandBuffer()->beginRenderPass(deferredGBufferComponent->pipeline->getRenderPass(), deferredGBufferComponent->framebuffers[frameComponent->swapchainImageIndex], clearValues, renderArea);
  frameComponent->getFrameGraphicsCommandBuffer()->bindPipeline(deferredGBufferComponent->pipeline);

  deferredGBufferComponent->perFrameBufferDescriptors[frameComponent->currentFrame].update(frameComponent->getFramePerFrameUniformBuffer());

  while (it.next())
  {
    auto meshBuffers = it.field<core::shared_ptr<Mesh_Buffer>>(0);
    auto meshes = it.field<core::shared_ptr<scene::Mesh>>(1);

    for (auto i : it)
    {
      deferredGBufferComponent->submeshesDrawsBufferDescriptor.update(meshBuffers[i]->getSubmeshesDrawsBuffer(), meshBuffers[i]->getSubmeshesDrawsBuffer()->getSize());
      deferredGBufferComponent->vertexBufferDescriptor.update(meshBuffers[i]->getVerticesBuffer(), meshBuffers[i]->getVerticesBuffer()->getSize());
      deferredGBufferComponent->meshletBufferDescriptor.update(meshBuffers[i]->getMeshletsBuffer(), meshBuffers[i]->getMeshletsBuffer()->getSize());
      deferredGBufferComponent->primitiveIndicesBufferDescriptor.update(meshBuffers[i]->getPrimitiveIndicesBuffer(), meshBuffers[i]->getPrimitiveIndicesBuffer()->getSize());
      deferredGBufferComponent->vertexIndicesBufferDescriptor.update(meshBuffers[i]->getVertexIndicesBuffer(), meshBuffers[i]->getVertexIndicesBuffer()->getSize());

      Per_Mesh perMesh;
      if (it.entity(i).has<scene::Transform>())
      {
        auto transform = it.entity(i).get_ref<scene::Transform>();
        perMesh.modelToWorld = transform->getNodeToWorldFloat4x4();
      }

      for (core::uint32 submeshIndex = 0u; submeshIndex < meshes[i]->submeshes.size(); ++submeshIndex)
      {
        const scene::Sub_Mesh& submesh = meshes[i]->submeshes[submeshIndex];
        deferredGBufferComponent->submeshTextureDescriptors[frameComponent->currentFrame].update(submesh.texture->getImageView(), submesh.texture->getSampler());

        core::array<VkWriteDescriptorSet, 7u> descriptorWrites;
        deferredGBufferComponent->perFrameBufferDescriptors[frameComponent->currentFrame].write(descriptorWrites[0]);
        deferredGBufferComponent->submeshTextureDescriptors[frameComponent->currentFrame].write(descriptorWrites[1]);
        deferredGBufferComponent->submeshesDrawsBufferDescriptor.write(descriptorWrites[2]);
        deferredGBufferComponent->vertexBufferDescriptor.write(descriptorWrites[3]);
        deferredGBufferComponent->meshletBufferDescriptor.write(descriptorWrites[4]);
        deferredGBufferComponent->primitiveIndicesBufferDescriptor.write(descriptorWrites[5]);
        deferredGBufferComponent->vertexIndicesBufferDescriptor.write(descriptorWrites[6]);

        frameComponent->getFrameGraphicsCommandBuffer()->pushDescriptorSet(deferredGBufferComponent->pipeline, descriptorWrites);
        perMesh.submeshIndex = submeshIndex;
        frameComponent->getFrameGraphicsCommandBuffer()->pushConstant(deferredGBufferComponent->pipeline->getPipelineLayout(), perMesh);
        frameComponent->getFrameGraphicsCommandBuffer()->drawMeshTasks(1u);
      }
    }
  }
  
  frameComponent->getFrameGraphicsCommandBuffer()->endRenderPass();
}

}