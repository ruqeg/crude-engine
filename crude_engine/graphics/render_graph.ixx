module;

#include <vulkan/vulkan.hpp>
#include <flecs.h>

export module crude.gfx.render_graph;

export import crude.gfx.renderer_frame;
export import crude.core.std_containers_heap;
export import crude.core.std_containers_stack;
export import crude.gfx.vk.subpass_dependency;
export import crude.gfx.vk.subpass_description;
export import crude.gfx.vk.attachment_description;
export import crude.gfx.vk.descriptor_set_layout_binding;
export import crude.gfx.vk.vulkan_object;
export import crude.gfx.vk.push_constant_range;
export import crude.gfx.vk.shader_stage_create_info;
export import crude.gfx.vk.color_blend_state_create_info;
export import crude.gfx.vk.uniform_buffer;
export import crude.gfx.vk.storage_buffer;
export import crude.gfx.vk.buffer_descriptor;
export import crude.gfx.vk.image_descriptor;
export import crude.gfx.vk.device;

export namespace crude::gfx::vk
{

class Device;
class Render_Pass;
class Framebuffer;
class Image_View;
class Pipeline;

}

export namespace crude::gfx
{

struct Attachment_Info
{
  VkExtent2D                                 extent          = { 800, 800 };
  VkFormat                                   format          = VK_FORMAT_UNDEFINED;
  VkSampleCountFlagBits                      samples         = VK_SAMPLE_COUNT_1_BIT;
  vk::Pipeline_Color_Blend_Attachment_State  blendAttachmentState;
};

class Render_Graph;

class Render_Pass
{
private:
  Render_Pass(core::shared_ptr<Render_Graph> graph, const VkExtent2D& framebufferExtent, VkPipelineStageFlags queue);
public:
  void addColorOutput(const Attachment_Info& colorOutput);
  void setDepthStencilOutput(const Attachment_Info& depthStencilOutput);
  [[nodiscard]] core::shared_ptr<vk::Uniform_Buffer_Descriptor[]> addUniformInput(VkShaderStageFlags stageFlags);
  [[nodiscard]] core::shared_ptr<vk::Storage_Buffer_Descriptor[]> addStorageInput(VkShaderStageFlags stageFlags);
  [[nodiscard]] core::shared_ptr<vk::Combined_Image_Sampler_Descriptor[]> addTextureInput(VkShaderStageFlags stageFlags);
  void setPushConstantRange(const vk::Push_Constant_Range_Base& pushConstantRange);
  void setShaderStagesInfo(const core::vector<vk::Shader_Stage_Create_Info>& shaderStagesInfo);
  void build(flecs::system renderPassSystem);
public:
  core::shared_ptr<vk::Pipeline> getPipeline();
private:
  void render();
private:
  void initializeRenderPass();
  void initializeFramebuffers();
  void initializePipeline();
private:
  flecs::system                                            m_renderPassSystem;
  core::shared_ptr<Render_Graph>                           m_graph;
  VkPipelineStageFlags                                     m_queue;
  core::string                                             m_name;
  core::vector<Attachment_Info>                            m_colorOutputs;
  core::optional<Attachment_Info>                          m_depthStencilOutput;
  core::shared_ptr<vk::Pipeline>                           m_pipeline;
  core::vector<core::shared_ptr<vk::Framebuffer>>          m_framebuffers;
  core::shared_ptr<vk::Render_Pass>                        m_renderPass;
  VkExtent2D                                               m_framebufferExtent;
  core::vector<vk::Descriptor_Set_Layout_Binding>          m_descriptorLayoutBindings;
  core::optional<vk::Push_Constant_Range_Base>             m_pushConstantRange;
  core::vector<vk::Shader_Stage_Create_Info>               m_shaderStagesInfo;
private:
  friend class Render_Graph;
};

class Render_Graph : public std::enable_shared_from_this<Render_Graph>
{
public:
  Render_Graph(core::shared_ptr<Renderer_Frame> rendererFrame, core::uint32 swapchainImagesCount);
public:
  void render();
  core::shared_ptr<Renderer_Frame> getRendererFrame();
  core::shared_ptr<Render_Pass> addPass(const VkExtent2D& framebufferExtent, VkPipelineStageFlags queue);
private:
  core::vector<core::weak_ptr<Render_Pass>>  m_passes;
  core::shared_ptr<vk::Device>               m_device;
  core::uint32                               m_swapchainImagesCount;
  core::shared_ptr<Renderer_Frame>           m_rendererFrame;
private:
  friend class Render_Pass;
};

}