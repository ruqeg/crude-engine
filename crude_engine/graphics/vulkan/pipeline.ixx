// !TODO Pipeline_Cahe
module;

#include <vulkan/vulkan.hpp>

export module crude_engine.graphics.vulkan.pipeline;

import crude_engine.core.shared_ptr;
import crude_engine.core.optional;
import crude_engine.core.std_containers_stack;
import crude_engine.graphics.vulkan.vulkan_object;
import crude_engine.graphics.vulkan.shader_stage_create_info;
import crude_engine.graphics.vulkan.vertex_input_state_create_info;
import crude_engine.graphics.vulkan.input_assembly_state_create_info;
import crude_engine.graphics.vulkan.viewport_state_create_info;
import crude_engine.graphics.vulkan.rasterization_state_create_info;
import crude_engine.graphics.vulkan.multisample_state_create_info;
import crude_engine.graphics.vulkan.depth_stencil_state_create_info;
import crude_engine.graphics.vulkan.color_blend_state_create_info;
import crude_engine.graphics.vulkan.dynamic_state_create_info;
import crude_engine.graphics.vulkan.tessellation_state_create_info;

export namespace crude_engine
{

class Device;
class Render_Pass;
class Pipeline_Layout;
class Pipeline;

class Pipeline : public Vulkan_Object<VkPipeline>
{
public:
  explicit Pipeline(Shared_Ptr<const Device>                           device,
                    Shared_Ptr<const Render_Pass>                      renderPass,
                    Shared_Ptr<const Pipeline_Layout>                  pipelineLayout,
                    Shared_Ptr<const Pipeline>                         basePipeline,
                    const span<Shader_Stage_Create_Info>&              shaderStages,
                    const Optional<Vertex_Input_State_Create_Info>&    vertexInputState,
                    const Optional<Tessellation_State_Create_Info>&    tessellationState,
                    const Optional<Input_Assembly_State_Create_Info>&  inputAsseblyState,
                    const Optional<Viewport_State_Create_Info>&        viewportState,
                    const Optional<Rasterization_State_Create_Info>&   rasterizationState,
                    const Optional<Multisample_State_Create_Info>&     multisampleState,
                    const Optional<Depth_Stencil_State_Create_Info>&   depthStencilState,
                    const Optional<Color_Blend_State_Create_Info>&     colorBlendState,
                    const Optional<Dynamic_State_Create_Info>&         dynamicState,
                    uint32                                             subpass);
  ~Pipeline();
  const VkPipelineBindPoint getBindPoint() const;
  Shared_Ptr<const Pipeline_Layout> getPipelineLayout();
private:
  Shared_Ptr<const Device>           m_device;
  Shared_Ptr<const Render_Pass>      m_renderPass;
  Shared_Ptr<const Pipeline_Layout>  m_pipelineLayout;
  Shared_Ptr<const Pipeline>         m_basePipeline;
  const VkPipelineBindPoint          m_bindPoint;
};

}