// !TODO Pipeline_Cahe
module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.pipeline;

import crude.graphics.vulkan_object;
export import crude.graphics.shader_stage_create_info;
export import crude.graphics.vertex_input_state_create_info;
export import crude.graphics.input_assembly_state_create_info;
export import crude.graphics.viewport_state_create_info;
export import crude.graphics.rasterization_state_create_info;
export import crude.graphics.multisample_state_create_info;
export import crude.graphics.depth_stencil_state_create_info;
export import crude.graphics.color_blend_state_create_info;
export import crude.graphics.dynamic_state_create_info;
export import crude.graphics.tessellation_state_create_info;
export import crude.core.optional;
export import crude.core.std_containers_stack;
export import crude.core.std_containers_heap;
export import crude.graphics.pipeline_layout;
export import crude.graphics.render_pass;

export namespace crude::graphics
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
                    const core::Optional<Vertex_Input_State_Create_Info>&    vertexInputState,
                    const core::Optional<Tessellation_State_Create_Info>&    tessellationState,
                    const core::Optional<Input_Assembly_State_Create_Info>&  inputAsseblyState,
                    const core::Optional<Viewport_State_Create_Info>&        viewportState,
                    const core::Optional<Rasterization_State_Create_Info>&   rasterizationState,
                    const core::Optional<Multisample_State_Create_Info>&     multisampleState,
                    const core::Optional<Depth_Stencil_State_Create_Info>&   depthStencilState,
                    const core::Optional<Color_Blend_State_Create_Info>&     colorBlendState,
                    const core::Optional<Dynamic_State_Create_Info>&         dynamicState,
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