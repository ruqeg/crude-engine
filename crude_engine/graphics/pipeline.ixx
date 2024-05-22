// !TODO Pipeline_Cahe
module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.vulkan.pipeline;

import crude.graphics.vulkan.vulkan_object;
import crude.graphics.vulkan.shader_stage_create_info;
import crude.graphics.vulkan.vertex_input_state_create_info;
import crude.graphics.vulkan.input_assembly_state_create_info;
import crude.graphics.vulkan.viewport_state_create_info;
import crude.graphics.vulkan.rasterization_state_create_info;
import crude.graphics.vulkan.multisample_state_create_info;
import crude.graphics.vulkan.depth_stencil_state_create_info;
import crude.graphics.vulkan.color_blend_state_create_info;
import crude.graphics.vulkan.dynamic_state_create_info;
import crude.graphics.vulkan.tessellation_state_create_info;
import crude.core.shared_ptr;
import crude.core.optional;
import crude.core.std_containers_stack;

export namespace crude::graphics
{

class Device;
class Render_Pass;
class Pipeline_Layout;

class Pipeline : public Vulkan_Object<VkPipeline>
{
public:
  explicit Pipeline(core::Shared_Ptr<const Device>                           device,
                    core::Shared_Ptr<const Render_Pass>                      renderPass,
                    core::Shared_Ptr<const Pipeline_Layout>                  pipelineLayout,
                    core::Shared_Ptr<const Pipeline>                         basePipeline,
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
  const VkPipelineBindPoint getBindPoint() const;
  core::Shared_Ptr<const Pipeline_Layout> getPipelineLayout();
private:
  core::Shared_Ptr<const Device>           m_device;
  core::Shared_Ptr<const Render_Pass>      m_renderPass;
  core::Shared_Ptr<const Pipeline_Layout>  m_pipelineLayout;
  core::Shared_Ptr<const Pipeline>         m_basePipeline;
  const VkPipelineBindPoint                m_bindPoint;
};

}