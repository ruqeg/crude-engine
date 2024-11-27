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

}

export namespace crude::gfx
{

struct Attachment_Info
{
  VkFormat               format = VK_FORMAT_UNDEFINED;
  VkSampleCountFlagBits  samples = VK_SAMPLE_COUNT_1_BIT;
};

enum Render_Graph_Queue_Flag_Bits
{
  RENDER_GRAPH_QUEUE_GRAPHICS_BIT = 1 << 0,
  RENDER_GRAPH_QUEUE_COMPUTE_BIT  = 1 << 1,
};

class Render_Resource {};
class Render_Texture_Resource : public Render_Resource
{
public:
  void setAttachmentInfo(const Attachment_Info& attachmentInfo) { m_attachmentInfo = attachmentInfo; }
  const Attachment_Info& getAttachmentInfo() const { return m_attachmentInfo; }
private:
  Attachment_Info m_attachmentInfo;
};

class Render_Graph;

class Render_Graph_Pass
{
  friend class Render_Graph;
public:
  Render_Graph_Pass() = default;
private:
  Render_Graph_Pass(Render_Graph& graph, core::uint32 index, VkPipelineStageFlags queue);
public:
  void setName(const core::string& name);
  void addColorOutput(const core::string& name, const Attachment_Info& attachmentInfo);
  void setDepthStencilOutput(const core::string& name, const Attachment_Info& attachmentInfo);
  void addAttachmentInput(const core::string& name, const Attachment_Info& attachmentInfo);
  void addTextureInput(const core::string& name, const Attachment_Info& attachmentInfo);
  void build();
private:
  Render_Graph&                                           m_graph;
  core::uint32                                            m_index;
  VkPipelineStageFlags                                    m_queue;
  core::string                                            m_name;
  std::vector<core::shared_ptr<Render_Texture_Resource>>  m_colorOutputs;
  core::shared_ptr<Render_Texture_Resource>               m_depthOutput;
  core::shared_ptr<vk::Render_Pass>                       m_renderPass;
};

class Render_Graph
{
  friend class Render_Graph_Pass;
public:
  Render_Graph(core::shared_ptr<vk::Device> device);
public:
  core::shared_ptr<Render_Graph_Pass> addPass(const core::string& name, VkPipelineStageFlags queue);
private:
  core::shared_ptr<Render_Texture_Resource> getTextureResource(const core::string& name);
private:
  core::vector<core::shared_ptr<Render_Graph_Pass>>        m_passes;
  core::vector<core::shared_ptr<Render_Texture_Resource>>  m_resources;
  core::unordered_map<core::string, core::uint32>          m_passToIndex;
  core::unordered_map<core::string, core::uint32>          m_resourceToIndex;
  core::shared_ptr<vk::Device>                             m_device;
};

}
