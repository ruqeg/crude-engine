// !TODO Pipeline_Cahe

#pragma once

#include "shader_stage_create_info.hpp"
#include "vertex_input_state_create_info.hpp"
#include "input_assembly_state_create_info.hpp"
#include "viewport_state_create_info.hpp"
#include "rasterization_state_create_info.hpp"
#include "multisample_state_create_info.hpp"
#include "depth_stencil_state_create_info.hpp"
#include "color_blend_state_create_info.hpp"
#include "dynamic_state_create_info.hpp"
#include "tessellation_state_create_info.hpp"
#include <optional>
#include <vector>

namespace crude_engine
{

class Device;
class Render_Pass;
class Pipeline_Layout;
class Pipeline;

struct Graphics_Pipeline_Create_Info
{
  static constexpr VkPipelineBindPoint                  bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

  std::shared_ptr<const Device>                         device;
  std::shared_ptr<const Render_Pass>                    renderPass;
  std::shared_ptr<const Pipeline_Layout>                pipelineLayout;
  std::shared_ptr<const Pipeline>                       basePipeline;
  std::vector<Shader_Stage_Create_Info>                 shaderStages;
  std::optional<Vertex_Input_State_Create_Info>         vertexInputState;
  std::optional<Tessellation_State_Create_Info>         tessellationState;
  std::optional<Input_Assembly_State_Create_Info>       inputAsseblyState;
  std::optional<Viewport_State_Create_Info>             viewportState;
  std::optional<Rasterization_State_Create_Info>        rasterizationState;
  std::optional<Multisample_State_Create_Info>          multisampleState;
  std::optional<Depth_Stencil_State_Create_Info>        depthStencilState;
  std::optional<Color_Blend_State_Create_Info>          colorBlendState;
  std::optional<Dynamic_State_Create_Info>              dynamicState;
  uint32                                                subpass;

  explicit Graphics_Pipeline_Create_Info(std::shared_ptr<const Device>                           device,
                                         std::shared_ptr<const Render_Pass>                      renderPass,
                                         std::shared_ptr<const Pipeline_Layout>                  pipelineLayout,
                                         std::shared_ptr<const Pipeline>                         basePipeline,
                                         const std::vector<Shader_Stage_Create_Info>&            shaderStages,
                                         const std::optional<Vertex_Input_State_Create_Info>&    vertexInputState,
                                         const std::optional<Tessellation_State_Create_Info>&    tessellationState,
                                         const std::optional<Input_Assembly_State_Create_Info>&  inputAsseblyState,
                                         const std::optional<Viewport_State_Create_Info>&        viewportState,
                                         const std::optional<Rasterization_State_Create_Info>&   rasterizationState,
                                         const std::optional<Multisample_State_Create_Info>&     multisampleState,
                                         const std::optional<Depth_Stencil_State_Create_Info>&   depthStencilState,
                                         const std::optional<Color_Blend_State_Create_Info>&     colorBlendState,
                                         const std::optional<Dynamic_State_Create_Info>&         dynamicState,
                                         uint32                                                  subpass);
};

class Pipeline : public TObject<VkPipeline>
{
public:
  explicit Pipeline(const Graphics_Pipeline_Create_Info& createInfo);
  ~Pipeline();
  const VkPipelineBindPoint getBindPoint() const;
  std::shared_ptr<const Pipeline_Layout> getPipelineLayout();
private:
  std::shared_ptr<const Device>           m_device;
  std::shared_ptr<const Render_Pass>      m_renderPass;
  std::shared_ptr<const Pipeline_Layout>  m_pipelineLayout;
  std::shared_ptr<const Pipeline>         m_basePipeline;
  const VkPipelineBindPoint               m_bindPoint;
};

}