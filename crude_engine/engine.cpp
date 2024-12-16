#include <DirectXMath.h>
#include <flecs.h>
#include <SDL3/SDL.h>
#include <vulkan/vulkan.h>
#include <functional>


#include <crude_shaders/deferred_gbuffer.frag.inl>
#include <crude_shaders/deferred_gbuffer.mesh.inl>
#include <crude_shaders/deferred_gbuffer.task.inl>


module crude.engine;

import crude.core.logger;
import crude.core.memory;
import crude.scripts.window_script;
import crude.gfx.vk.device;
import crude.platform.sdl_helper;
import crude.platform.sdl_window_container;
import crude.gui.imgui_helper;
import crude.gui.imgui_input_system;
import crude.scene.mesh;
import crude.scene.light;



import crude.gfx.vk.format_helper;
import crude.gfx.render_graph;
import crude.gfx.vk.shader_module;
import crude.gfx.vk.shader_stage_create_info;
import crude.gfx.vk.push_constant_range;
import crude.gfx.vk.color_blend_state_create_info;
import crude.gfx.vk.image_descriptor;
import crude.gfx.mesh_buffer;
import crude.gfx.material;
import crude.gfx.camera_ubo;
import crude.gfx.texture;

namespace crude
{

void Engine::preinitialize(core::uint32 defaultFreeRBTCapacity)
{
  core::initializeMemory(defaultFreeRBTCapacity);
  platform::initializeSDL();
  platform::loadSDLVulkan();
  gui::initializeImGuiContext();
}

void Engine::postdeinitialize()
{
  gui::deinitializeImGuiContext();
  platform::deinitializeSDL();
  platform::unloadSDLVulkan();
}

void Engine::initialize(const Initialize& initialize)
{
  initializeWindow(initialize.window);
  initializeSystems();
}

void Engine::deinitialize()
{
}

void Engine::mainLoop()
{
  const auto windowScriptComponent = m_world.get<scripts::Window_Script_Component>();
  //while (!windowScriptComponent->shouldClose)
  //{
  //  m_world.progress();
  //}

  const VkFormat depthFormat = gfx::vk::findDepthFormatSupportedByDevice(m_rendererCore->getDevice()->getPhysicalDevice(), gfx::vk::depth_formats::gDepthCandidates);

  crude::gfx::Attachment_Info albedo, normal, metallicRoughness, depth;
  albedo.format = VK_FORMAT_B8G8R8A8_SRGB;
  albedo.blendAttachmentState = gfx::vk::Pipeline_Color_Blend_Attachment_State({
    .blendEnable         = VK_TRUE,
    .colorWriteMask      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
    .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
    .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
    .colorBlendOp        = VK_BLEND_OP_ADD,
    .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
    .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
    .alphaBlendOp        = VK_BLEND_OP_ADD });
 
  normal.format = VK_FORMAT_R16G16B16A16_SNORM;
  normal.blendAttachmentState = gfx::vk::Pipeline_Color_Blend_Attachment_State({
    .blendEnable         = VK_FALSE,
    .colorWriteMask      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT,
    .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
    .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
    .colorBlendOp        = VK_BLEND_OP_ADD,
    .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
    .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
    .alphaBlendOp        = VK_BLEND_OP_ADD });

  metallicRoughness.format = VK_FORMAT_R8G8_UNORM;
  metallicRoughness.blendAttachmentState = gfx::vk::Pipeline_Color_Blend_Attachment_State({
    .blendEnable         = VK_FALSE,
    .colorWriteMask      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
    .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
    .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
    .colorBlendOp        = VK_BLEND_OP_ADD,
    .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
    .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
    .alphaBlendOp        = VK_BLEND_OP_ADD });

  depth.format             = depthFormat;

  //core::shared_ptr<gfx::Render_Graph> graph = core::allocateShared<gfx::Render_Pass>(gfx::Render_Graph::Initialize{

  //core::vector<core::shared_ptr<Render_Pass>>  passes;
  //core::shared_ptr<vk::Device>                 device;
  //core::uint32                                 swapchainImagesCount;

  //  });
  
  struct Per_Mesh
  {
    // !TODO Move
    DirectX::XMFLOAT4X4  modelToWorld;
    core::uint32         submeshIndex;
  };
  core::shared_ptr<gfx::Render_Graph> graph = core::allocateShared<gfx::Render_Graph>(m_rendererFrame, 3u);
  core::shared_ptr<gfx::Render_Pass> gbuffer = graph->addPass(VkExtent2D{ 800, 800 }, VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT);
  gbuffer->addColorOutput(albedo);
  gbuffer->addColorOutput(normal);
  gbuffer->addColorOutput(metallicRoughness);
  gbuffer->setDepthStencilOutput(depth);
  gbuffer->setPushConstantRange(gfx::vk::Push_Constant_Range<Per_Mesh>(VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT));
  gbuffer->setShaderStagesInfo({
    gfx::vk::Task_Shader_Stage_Create_Info(core::allocateShared<gfx::vk::Shader_Module>(m_rendererCore->getDevice(), crude::shaders::deferred_gbuffer::task), "main"),
    gfx::vk::Mesh_Shader_Stage_Create_Info(core::allocateShared<gfx::vk::Shader_Module>(m_rendererCore->getDevice(), crude::shaders::deferred_gbuffer::mesh), "main"),
    gfx::vk::Fragment_Shader_Stage_Create_Info(core::allocateShared<gfx::vk::Shader_Module>(m_rendererCore->getDevice(), crude::shaders::deferred_gbuffer::frag), "main"),
  });
  auto perFramesDesc = gbuffer->addUniformInput("Per Frame", VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_FRAGMENT_BIT);
  auto submeshesDrawsDesc = gbuffer->addStorageInput("Submeshes Draws", VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT);
  auto verticesDesc = gbuffer->addStorageInput("Vertices", VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT);
  auto meshletsDesc = gbuffer->addStorageInput("Meshlets", VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT);
  auto primitiveIndicesDesc = gbuffer->addStorageInput("Primitive Indices", VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT);
  auto vertexIndicesDesc = gbuffer->addStorageInput("Vertex Indices", VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT);
  auto albedoDesc = gbuffer->addTextureInput("Albedo", VK_SHADER_STAGE_FRAGMENT_BIT);
  auto metallicRoughnessDesc = gbuffer->addTextureInput("Metallic-Roughness", VK_SHADER_STAGE_FRAGMENT_BIT);
  auto normalDesc = gbuffer->addTextureInput("Normal", VK_SHADER_STAGE_FRAGMENT_BIT);

  struct Per_Frame
  {
    gfx::Camera_UBO camera;
  };
  core::vector<core::shared_ptr<gfx::vk::Uniform_Buffer<Per_Frame>>> perFrameUniformBuffers =
  {
    core::allocateShared<gfx::vk::Uniform_Buffer<Per_Frame>>(m_rendererCore->getDevice()),
    core::allocateShared<gfx::vk::Uniform_Buffer<Per_Frame>>(m_rendererCore->getDevice()),
    core::allocateShared<gfx::vk::Uniform_Buffer<Per_Frame>>(m_rendererCore->getDevice())
  };
  gbuffer->build(m_world
    .system<core::shared_ptr<gfx::Mesh_Buffer>, core::shared_ptr<scene::Mesh>>("gbuffer_pbr_pass_system")
    .kind(0)
    .run([rendererFrame = m_rendererFrame, gbuffer, perFrameUniformBuffers, perFramesDesc, submeshesDrawsDesc, verticesDesc, meshletsDesc, primitiveIndicesDesc, vertexIndicesDesc, albedoDesc, metallicRoughnessDesc, normalDesc](flecs::iter& it) {
      const core::uint32 currentFrame = rendererFrame->getCurrentFrame();
      perFramesDesc[currentFrame].updateBase(perFrameUniformBuffers[currentFrame], perFrameUniformBuffers[currentFrame]->getSize());

      while (it.next())
      {
        auto meshBuffers = it.field<core::shared_ptr<gfx::Mesh_Buffer>>(0);
        auto meshes = it.field<core::shared_ptr<scene::Mesh>>(1);

        for (auto i : it)
        {
          submeshesDrawsDesc[currentFrame].updateBase(meshBuffers[i]->getSubmeshesDrawsBuffer(), meshBuffers[i]->getSubmeshesDrawsBuffer()->getSize());
          verticesDesc[currentFrame].updateBase(meshBuffers[i]->getVerticesBuffer(), meshBuffers[i]->getVerticesBuffer()->getSize());
          meshletsDesc[currentFrame].updateBase(meshBuffers[i]->getMeshletsBuffer(), meshBuffers[i]->getMeshletsBuffer()->getSize());
          primitiveIndicesDesc[currentFrame].updateBase(meshBuffers[i]->getPrimitiveIndicesBuffer(), meshBuffers[i]->getPrimitiveIndicesBuffer()->getSize());
          vertexIndicesDesc[currentFrame].updateBase(meshBuffers[i]->getVertexIndicesBuffer(), meshBuffers[i]->getVertexIndicesBuffer()->getSize());

          Per_Mesh perMesh;
          if (it.entity(i).has<scene::Transform>())
          {
            auto transform = it.entity(i).get_ref<scene::Transform>();
            perMesh.modelToWorld = transform->getNodeToWorldFloat4x4();
          }

          for (core::uint32 submeshIndex = 0u; submeshIndex < meshes[i]->submeshes.size(); ++submeshIndex)
          {
            const scene::Sub_Mesh& submesh = meshes[i]->submeshes[submeshIndex];
            // !TODO ???
            if (!submesh.material || !submesh.material->albedo || !submesh.material->metallicRoughness || !submesh.material->normal)
            {
              continue;
            }
            albedoDesc[rendererFrame->getCurrentFrame()].update(submesh.material->albedo->getImageView(), submesh.material->albedo->getSampler());
            metallicRoughnessDesc[rendererFrame->getCurrentFrame()].update(submesh.material->metallicRoughness->getImageView(), submesh.material->metallicRoughness->getSampler());
            normalDesc[rendererFrame->getCurrentFrame()].update(submesh.material->normal->getImageView(), submesh.material->normal->getSampler());

            perMesh.submeshIndex = submeshIndex;
            gbuffer->pushDescriptorSet();
            gbuffer->pushConstant(perMesh);

            rendererFrame->getGraphicsCommandBuffer()->drawMeshTasks(1);
          }
        }
      }
      }));

  gbuffer->run();
  //auto renderer = Util::make_handle<RenderPassSceneRenderer>();

  //RenderPassSceneRenderer::Setup setup = {};
  //setup.scene           = &scene_loader.get_scene();
  //setup.deferred_lights = &deferred_lights;
  //setup.context         = &context;
  //setup.suite           = &renderer_suite;
  //setup.flags           = SCENE_RENDERER_DEFERRED_GBUFFER_BIT;
  ////if (!config.clustered_lights && config.deferred_clustered_stencil_culling)
  ////  setup.flags |= SCENE_RENDERER_DEFERRED_GBUFFER_LIGHT_PREPASS_BIT;
  ////if (config.debug_probes)
  ////  setup.flags |= SCENE_RENDERER_DEBUG_PROBES_BIT;

  //renderer->init(setup);

  //gbuffer.set_render_pass_interface(std::move(renderer));

  //gbuffer->build();

  ////auto lighting = graph.addPass("lighting", VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT);
  ////lighting->addColorOutput("HDR", emissive, "emissive");
  ////lighting->addAttachmentInput("albedo");
  ////lighting->addAttachmentInput("normal");
  ////lighting->addAttachmentInput("pbr");
  ////lighting->addAttachmentInput("depth");
  ////lighting->setDepthStencilInput("depth");

  ////lighting->addTextureInput("shadow-main");
  ////lighting->addTextureInput("shadow-near");

  m_rendererCore->getDevice()->waitIdle();
}

void Engine::initializeWindow(const Initialize_Window& initialize)
{
  m_windowContainer = crude::core::allocateShared<crude::platform::SDL_Window_Container>(
    initialize.title, initialize.width, initialize.height, crude::platform::SDL_WINDOW_CONTAINER_FLAG_VULKAN);
}

void Engine::initializeSystems()
{
  initializeInputSystems();
  initializeRendererSystems();

  m_sceneLoaderCtx = core::allocateShared<resources::Scene_Loader_Context>();
  m_sceneSaverCtx = core::allocateShared<resources::Scene_Saver_Context>();
  m_sceneSaverSystem = resources::registerSceneSaverSystem(m_world, m_sceneSaverCtx);
  m_sceneLoaderSystem = resources::registerSceneLoaderSystem(m_world, m_sceneLoaderCtx);

  m_gltfModelLoaderCtx = core::allocateShared<resources::GLTF_Model_Loader_Context>(resources::GLTF_Model_Loader_Context{
    .transferCommandPool = m_rendererCore->getTransferCommandPool(),
    .callback = [](flecs::entity entity) {
    }});
  m_gltfModelLoaderSystem = resources::registerGLTFModelLoaderSystem(m_world, m_gltfModelLoaderCtx);
}

void Engine::initializeInputSystems()
{
  m_world.set<scripts::Window_Script_Component>({});

  m_inputSystemCtx = core::allocateShared<platform::Input_System_Context>();
  m_inputSystemCtx->handleEventSystems = {
    m_world.system("WindowInputSystem")
      .kind(0)
      .ctx(m_inputSystemCtx.get())
      .run(scripts::windowScriptInputSystemProcess),
    m_world.system("ImguiUpdateEvent")
      .kind(0)
      .ctx(m_inputSystemCtx.get())
      .run(gui::imguiInputSystemProcess)
  };

  m_inputSystem = m_world.system("InputSystem")
    .ctx(m_inputSystemCtx.get())
    .kind(flecs::PreUpdate)
    .run(platform::inputSystemProcess);
}

void Engine::initializeRendererSystems()
{
  m_rendererCore = core::allocateShared<gfx::Renderer_Core>(m_windowContainer);
  m_rendererFrame = core::allocateShared<gfx::Renderer_Frame>(m_rendererCore, 2u);
}

}