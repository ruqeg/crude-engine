module;

#include <vulkan/vulkan.hpp>

export module crude.gfx.vk.graphics_pipeline;

export import crude.gfx.vk.pipeline;

import crude.gfx.vk.vulkan_object;
export import crude.gfx.vk.shader_stage_create_info;
export import crude.gfx.vk.vertex_input_state_create_info;
export import crude.gfx.vk.input_assembly_state_create_info;
export import crude.gfx.vk.viewport_state_create_info;
export import crude.gfx.vk.rasterization_state_create_info;
export import crude.gfx.vk.multisample_state_create_info;
export import crude.gfx.vk.depth_stencil_state_create_info;
export import crude.gfx.vk.color_blend_state_create_info;
export import crude.gfx.vk.dynamic_state_create_info;
export import crude.gfx.vk.tessellation_state_create_info;
export import crude.core.std_containers_stack;
export import crude.core.std_containers_heap;

export namespace crude::gfx::vk
{

class Device;
class Render_Pass;

class Graphics_Pipeline : public Pipeline
{
public:
  struct Description;
public:
  explicit Graphics_Pipeline(core::shared_ptr<Render_Pass> renderPass, core::shared_ptr<Pipeline_Layout> pipelineLayout, const Description& description);
  ~Graphics_Pipeline() = default;
public:
  core::shared_ptr<Render_Pass> getRenderPass();
private:
  core::shared_ptr<Render_Pass> m_renderPass;
  core::shared_ptr<Pipeline>    m_basePipeline;
};

struct Graphics_Pipeline::Description
{
  core::vector<Shader_Stage_Create_Info>             shaderStages;
  core::optional<Vertex_Input_State_Create_Info>     vertexInputState;
  core::optional<Tessellation_State_Create_Info>     tessellationState;
  core::optional<Input_Assembly_State_Create_Info>   inputAsseblyState;
  core::optional<Viewport_State_Create_Info>         viewportState;
  core::optional<Rasterization_State_Create_Info>    rasterizationState;
  core::optional<Multisample_State_Create_Info>      multisampleState;
  core::optional<Depth_Stencil_State_Create_Info>    depthStencilState;
  core::optional<Color_Blend_State_Create_Info>      colorBlendState;
  core::optional<Dynamic_State_Create_Info>          dynamicState;
};

}