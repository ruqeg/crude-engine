#include <vulkan/vulkan.hpp>
#include <flecs.h>
#include <cstring>
#include <algorithm>
#include <directxmath/DirectXMath.h>
#include <crude_shaders/shader.frag.inl>
#include <crude_shaders/shader.mesh.inl>
#include <crude_shaders/shader.task.inl>
#include <functional>

module crude.graphics.renderer_geometry_system;

import crude.core.logger;
import crude.scene.image;
import crude.graphics.format_helper;
import crude.graphics.generate_mipmaps;
import crude.graphics.flush;
import crude.graphics.mesh_buffer;
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

namespace crude::graphics
{

constexpr core::uint32       cPerFrameUniformBufferBinding = 0u;
constexpr VkShaderStageFlags cPerFrameUniformBufferStageFlags = VK_SHADER_STAGE_MESH_BIT_EXT;

constexpr core::uint32       cSubmeshesDrawsBufferBinding = 1u;
constexpr VkShaderStageFlags cSubmeshesDrawsBufferStageFlags = VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT;

constexpr core::uint32       cMeshletBufferBinding = 2u;
constexpr VkShaderStageFlags cMeshletBufferStageFlags = VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT;

constexpr core::uint32       cVertexBufferBinding = 3u;
constexpr VkShaderStageFlags cVertexBufferStageFlags = VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT;

constexpr core::uint32       cPrimitiveIndicesBufferBinding = 4u;
constexpr VkShaderStageFlags cPrimitiveIndicesBufferStageFlags = VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT;

constexpr core::uint32       cVertexIndicesBufferBinding = 5u;
constexpr VkShaderStageFlags cVertexIndicesBufferStageFlags = VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT;

constexpr core::uint32       cTextureSamplerBinding = 6u;
constexpr VkShaderStageFlags cTextureSamplerStageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

const Uniform_Buffer_Descriptor          cPerFrameUniformBufferDescriptor(cPerFrameUniformBufferBinding, cPerFrameUniformBufferStageFlags);
const Combined_Image_Sampler_Descriptor  cTextureSamplerDescriptor(cTextureSamplerBinding, cTextureSamplerStageFlags);
const Storage_Buffer_Descriptor          cSubmeshesDrawsBufferDescriptor{ cSubmeshesDrawsBufferBinding, cSubmeshesDrawsBufferStageFlags };
const Storage_Buffer_Descriptor          cVertexBufferDescriptor{ cVertexBufferBinding, cVertexBufferStageFlags };
const Storage_Buffer_Descriptor          cMeshletBufferDescriptor{ cMeshletBufferBinding, cMeshletBufferStageFlags };
const Storage_Buffer_Descriptor          cPrimitiveIndicesBufferDescriptor{ cPrimitiveIndicesBufferBinding, cPrimitiveIndicesBufferStageFlags };
const Storage_Buffer_Descriptor          cVertexIndicesBufferDescriptor{ cVertexIndicesBufferBinding, cVertexIndicesBufferStageFlags };

core::array<Descriptor_Set_Layout_Binding, 7u> cDescriptorLayoutBindings = 
{
  cPerFrameUniformBufferDescriptor,
  cTextureSamplerDescriptor,
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

Renderer_Geometry_Component::Renderer_Geometry_Component(core::shared_ptr<Device>                    device,
                                                         core::shared_ptr<Color_Attachment>          colorAttachment,
                                                         core::shared_ptr<Depth_Stencil_Attachment>  depthStencilAttachment)
  : perFrameUniformBufferDescriptors{ cPerFrameUniformBufferDescriptor, cPerFrameUniformBufferDescriptor }
  , textureSamplerDescriptors{ cTextureSamplerDescriptor, cTextureSamplerDescriptor }
  , submeshesDrawsBufferDescriptor{ cSubmeshesDrawsBufferDescriptor }
  , vertexBufferDescriptor{ cVertexBufferDescriptor }
  , meshletBufferDescriptor{ cMeshletBufferDescriptor }
  , primitiveIndicesBufferDescriptor{ cPrimitiveIndicesBufferDescriptor }
  , vertexIndicesBufferDescriptor{ cVertexIndicesBufferDescriptor }
{
  initializeRenderPass(device, colorAttachment, depthStencilAttachment);
  initalizeGraphicsPipeline(device);
  initializeFramebuffers(device, colorAttachment, depthStencilAttachment);
}

core::shared_ptr<Descriptor_Set_Layout> Renderer_Geometry_Component::createDescriptorSetLayout(core::shared_ptr<Device> device)
{
  auto descriptorPool = core::allocateShared<Descriptor_Pool>(device, cDescriptorPoolSizes);
  auto descriptorSetLayout = core::allocateShared<Descriptor_Set_Layout>(device, cDescriptorLayoutBindings, true);
  return descriptorSetLayout;
}

void Renderer_Geometry_Component::initializeRenderPass(core::shared_ptr<Device> device, core::shared_ptr<Color_Attachment> colorAttachment, core::shared_ptr<Depth_Stencil_Attachment> depthStencilAttachment)
{
  core::array<Subpass_Description, 1u> subpasses =
  {
    Subpass_Description(
      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
  };

  core::array<Subpass_Dependency, 1u> subpassesDependencies =
  {
    Subpass_Dependency(
      VK_SUBPASS_EXTERNAL, 0u,
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
      0u,
      VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
      0u)
  };

  core::array<Attachment_Description, 2> attachments =
  {
    Color_Attachment_Description({
      .attachment = colorAttachment}),
    Depth_Attachment_Description({
      .attachment = depthStencilAttachment }),
  };

  renderPass = core::allocateShared<Render_Pass>(device, subpasses, subpassesDependencies, attachments);
}

void Renderer_Geometry_Component::initalizeGraphicsPipeline(core::shared_ptr<Device> device)
{
  core::shared_ptr<Shader_Module> taskShaderModule = core::allocateShared<Shader_Module>(device, crude::shaders::shader::task);
  core::shared_ptr<Shader_Module> meshShaderModule = core::allocateShared<Shader_Module>(device, crude::shaders::shader::mesh);
  core::shared_ptr<Shader_Module> fragShaderModule = core::allocateShared<Shader_Module>(device, crude::shaders::shader::frag);

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
    .rasterizationSamples = device->getPhysicalDevice()->getProperties().getMaximumUsableSampleCount(),
    .sampleShadingEnable = VK_TRUE,
    .minSampleShading = 0.2f,
    .alphaToCoverageEnable = false,
    .alphaToOneEnable = false });

  Depth_Stencil_State_Create_Info depthStencil({
    .depthTestEnable = true,
    .depthWriteEnable = true,
    .depthCompareOp = VK_COMPARE_OP_LESS,
    .depthBoundsTestEnable = false,
    .stencilTestEnable = false,
    .front = {},
    .back = {},
    .minDepthBounds = 0.0f,
    .maxDepthBounds = 1.0f });

  core::array<Pipeline_Color_Blend_Attachment_State, 1u> colorBlendAttachments = {
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

  Color_Blend_State_Create_Info colorBlending({
    .attachments = colorBlendAttachments,
    .blendConstants = { 0.f, 0.f, 0.f, 0.f },
    .logicOpEnable = false,
    .logicOp = VK_LOGIC_OP_COPY });

  core::array<VkDynamicState, 2> dynamicStates = {
    VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR
  };
  Dynamic_State_Create_Info dynamicStateInfo(dynamicStates);

  core::shared_ptr<Pipeline_Layout> pipelineLayout = core::allocateShared<Pipeline_Layout>(
    device, createDescriptorSetLayout(device),
    Push_Constant_Range<Per_Mesh>(VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT));

  pipeline = core::allocateShared<Pipeline>(
    device,
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
    depthStencil,
    colorBlending,
    dynamicStateInfo,
    0u);
}

void Renderer_Geometry_Component::initializeFramebuffers(core::shared_ptr<Device> device, core::shared_ptr<Color_Attachment> colorAttachment, core::shared_ptr<Depth_Stencil_Attachment> depthStencilAttachment)
{
  framebuffers.reserve(cFramesCount);
  for (core::uint32 i = 0; i < cFramesCount; ++i)
  {
    core::shared_ptr<Image_View> colorAttachmentView = core::allocateShared<Image_View>(colorAttachment, Image_Subresource_Range(colorAttachment));
    core::shared_ptr<Image_View> depthStencilAttachmentView = core::allocateShared<Image_View>(depthStencilAttachment, Image_Subresource_Range(depthStencilAttachment));
    core::vector<core::shared_ptr<Image_View>> attachments = { colorAttachmentView, depthStencilAttachmentView };
    framebuffers.push_back(core::allocateShared<Framebuffer>(device, pipeline->getRenderPass(), attachments, colorAttachment->getExtent2D(), 1u));
  }
}

void renderGeometrySystemEach(flecs::iter& it, size_t index, core::shared_ptr<scene::Mesh> mesh, core::shared_ptr<Mesh_Buffer> meshBuffer);

void renderGeometrySystemProcess(flecs::iter& it)
{
  Renderer_Geometry_Component* geometryComponent = it.world().get_mut<Renderer_Geometry_Component>();
  Renderer_Frame_Component* frameComponent = it.world().get_mut<Renderer_Frame_Component>();
  Renderer_Core_Component* coreComponent = it.world().get_mut<Renderer_Core_Component>();
  
  core::array<VkClearValue, 2u> clearValues;;
  clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
  clearValues[1].depthStencil = { 1.0f, 0 };

  VkRect2D renderArea;
  renderArea.extent = coreComponent->swapchain->getExtent();
  renderArea.offset = VkOffset2D { 0, 0 };

  frameComponent->getFrameGraphicsCommandBuffer()->beginRenderPass(geometryComponent->pipeline->getRenderPass(), geometryComponent->framebuffers[frameComponent->currentFrame], clearValues, renderArea);
  frameComponent->getFrameGraphicsCommandBuffer()->bindPipeline(geometryComponent->pipeline);

  it.world().query<core::shared_ptr<scene::Mesh>, core::shared_ptr<Mesh_Buffer>>().each(std::function(renderGeometrySystemEach));

  frameComponent->getFrameGraphicsCommandBuffer()->endRenderPass();
}

void renderGeometrySystemEach(flecs::iter& it, size_t index, core::shared_ptr<scene::Mesh> mesh, core::shared_ptr<Mesh_Buffer> meshBuffer)
{
  Renderer_Geometry_Component* geometryComponent = it.world().get_mut<Renderer_Geometry_Component>();
  Renderer_Frame_Component* frameComponent = it.world().get_mut<Renderer_Frame_Component>();
  Renderer_Core_Component* coreComponent = it.world().get_mut<Renderer_Core_Component>();

  geometryComponent->submeshesDrawsBufferDescriptor.update(meshBuffer->getSubmeshesDrawsBuffer(), meshBuffer->getSubmeshesDrawsBuffer()->getSize());
  geometryComponent->vertexBufferDescriptor.update(meshBuffer->getVerticesBuffer(), meshBuffer->getVerticesBuffer()->getSize());
  geometryComponent->meshletBufferDescriptor.update(meshBuffer->getMeshletsBuffer(), meshBuffer->getMeshletsBuffer()->getSize());
  geometryComponent->primitiveIndicesBufferDescriptor.update(meshBuffer->getPrimitiveIndicesBuffer(), meshBuffer->getPrimitiveIndicesBuffer()->getSize());
  geometryComponent->vertexIndicesBufferDescriptor.update(meshBuffer->getVertexIndicesBuffer(), meshBuffer->getVertexIndicesBuffer()->getSize());

  Per_Mesh perMesh;
  if (it.entity(index).has<scene::Transform>())
  {
    auto transform = it.entity(index).get_ref<scene::Transform>();
    perMesh.modelToWorld = transform->getNodeToWorldFloat4x4();
  }

  for (core::uint32 submeshIndex = 0u; submeshIndex < mesh->submeshes.size(); ++submeshIndex)
  {
    const scene::Sub_Mesh& submesh = mesh->submeshes[submeshIndex];
    geometryComponent->textureSamplerDescriptors[frameComponent->currentFrame].update(submesh.texture->getImageView(), submesh.texture->getSampler());

    core::array<VkWriteDescriptorSet, 7u> descriptorWrites;
    geometryComponent->perFrameBufferDescriptors[frameComponent->currentFrame].write(descriptorWrites[0]);
    geometryComponent->textureSamplerDescriptors[frameComponent->currentFrame].write(descriptorWrites[1]);
    geometryComponent->submeshesDrawsBufferDescriptor.write(descriptorWrites[2]);
    geometryComponent->vertexBufferDescriptor.write(descriptorWrites[3]);
    geometryComponent->meshletBufferDescriptor.write(descriptorWrites[4]);
    geometryComponent->primitiveIndicesBufferDescriptor.write(descriptorWrites[5]);
    geometryComponent->vertexIndicesBufferDescriptor.write(descriptorWrites[6]);

    frameComponent->getFrameGraphicsCommandBuffer()->pushDescriptorSet(geometryComponent->pipeline, descriptorWrites);
    perMesh.submeshIndex = submeshIndex;
    frameComponent->getFrameGraphicsCommandBuffer()->pushConstant(geometryComponent->pipeline->getPipelineLayout(), perMesh);
    frameComponent->getFrameGraphicsCommandBuffer()->drawMeshTasks(1u);
  }
}

}