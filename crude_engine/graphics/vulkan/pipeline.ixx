// !TODO Pipeline_Cahe
module;

#include <vulkan/vulkan.hpp>

export module crude.gfx.vk.pipeline;

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
export import crude.gfx.vk.pipeline_layout;
export import crude.gfx.vk.render_pass;

export namespace crude::gfx::vk
{

class Device;

class Pipeline : public Vulkan_Object<VkPipeline>
{
public:
  explicit Pipeline(core::shared_ptr<const Device>                           device,
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
                    core::uint32                                             subpass);
  ~Pipeline();
  const VkPipelineBindPoint getBindPoint() const { return m_bindPoint; }
  core::shared_ptr<const Pipeline_Layout> getPipelineLayout() const { return m_pipelineLayout; }
  core::shared_ptr<Render_Pass> getRenderPass() { return m_renderPass; }
private:
  core::shared_ptr<const Device>           m_device;
  core::shared_ptr<Render_Pass>            m_renderPass;
  core::shared_ptr<const Pipeline_Layout>  m_pipelineLayout;
  core::shared_ptr<const Pipeline>         m_basePipeline;
  const VkPipelineBindPoint                m_bindPoint;
};

}