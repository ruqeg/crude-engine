// !TODO Pipeline_Cahe

#pragma once

#include <core/data_structures/shared_ptr.hpp>
#include <core/data_structures/array_unsafe.hpp>
#include <core/data_structures/optional.hpp>
#include <graphics/vulkan/shader_stage_create_info.hpp>
#include <graphics/vulkan/vertex_input_state_create_info.hpp>
#include <graphics/vulkan/input_assembly_state_create_info.hpp>
#include <graphics/vulkan/viewport_state_create_info.hpp>
#include <graphics/vulkan/rasterization_state_create_info.hpp>
#include <graphics/vulkan/multisample_state_create_info.hpp>
#include <graphics/vulkan/depth_stencil_state_create_info.hpp>
#include <graphics/vulkan/color_blend_state_create_info.hpp>
#include <graphics/vulkan/dynamic_state_create_info.hpp>
#include <graphics/vulkan/tessellation_state_create_info.hpp>
#include <graphics/vulkan/object.hpp>

namespace crude_engine
{

class Device;
class Render_Pass;
class Pipeline_Layout;
class Pipeline;

class Pipeline : public TObject<VkPipeline>
{
public:
  explicit Pipeline(Shared_Ptr<const Device>                           device,
                    Shared_Ptr<const Render_Pass>                      renderPass,
                    Shared_Ptr<const Pipeline_Layout>                  pipelineLayout,
                    Shared_Ptr<const Pipeline>                         basePipeline,
                    const Array_Unsafe<Shader_Stage_Create_Info>&      shaderStages,
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