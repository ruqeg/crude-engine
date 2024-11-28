module;

#include <vulkan/vulkan.hpp>

export module crude.gfx.render_graph;

export import crude.core.std_containers_heap;
export import crude.core.std_containers_stack;
import crude.gfx.vk.vulkan_object;
export import crude.gfx.vk.subpass_dependency;
export import crude.gfx.vk.subpass_description;
export import crude.gfx.vk.attachment_description;


export namespace crude::gfx::vk
{

class Device;
class Render_Pass;
class Framebuffer;
class Image_View;

}

export namespace crude::gfx
{

struct Attachment_Info
{
  VkExtent2D             extent          = { 800, 800 };
  VkFormat               format          = VK_FORMAT_UNDEFINED;
  VkSampleCountFlagBits  samples         = VK_SAMPLE_COUNT_1_BIT;
};

class Render_Graph;

class Render_Pass
{
private:
  struct Initialize;
private:
  Render_Pass(const Initialize& info);
private:
  Render_Graph&                                    m_graph;
  VkPipelineStageFlags                             m_queue;
  core::string                                     m_name;
  std::vector<core::shared_ptr<vk::Image_View>>    m_outputs;
  core::shared_ptr<vk::Render_Pass>                m_renderPass;
  core::vector<core::shared_ptr<vk::Framebuffer>>  m_framebuffers;
  VkExtent2D                                       m_framebufferExtent;
private:
  friend class Render_Graph;
};

class Render_Graph
{
public:
  struct Initialize;
  struct Render_Pass_Info;
public:
  Render_Graph(const Initialize& initialize);
private:
  core::vector<core::shared_ptr<Render_Pass>>  m_passes;
  core::shared_ptr<vk::Device>                 m_device;
  core::uint32                                 m_swapchainImagesCount;
private:
  friend class Render_Pass;
};

struct Render_Pass::Initialize
{
  Render_Graph&                      graph;
  VkPipelineStageFlags               queue;
  core::string                       name;
  core::span<const Attachment_Info>  colorAttachments;
  core::optional<Attachment_Info>    depthAttachment;
  VkExtent2D                         framebufferExtent;
};

struct Render_Graph::Render_Pass_Info
{
  core::string                     name;
  VkPipelineStageFlags             queue;
  core::vector<Attachment_Info>    colorAttachments;
  core::optional<Attachment_Info>  depthAttachment;
  VkExtent2D                       framebufferExtent;
};

struct Render_Graph::Initialize
{
  core::vector<Render_Pass_Info>  renderPassesInfos;
  core::shared_ptr<vk::Device>    device;
  core::uint32                    swapchainImagesCount;
};

}
