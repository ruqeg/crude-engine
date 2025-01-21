module;

#include <vulkan/vulkan.h>

export module crude.gfx.frame_graph;

export import crude.core.std_containers_heap;
export import crude.gfx.renderer_frame;

export namespace crude::gfx::vk
{

class Render_Pass;
class Framebuffer;
class Buffer;
class Image;

}

export namespace crude::gfx
{

using Frame_Graph_Handle = core::uint32;

struct Frame_Graph_Resource_Handle
{
  Frame_Graph_Handle index;
};

struct Frame_Graph_Node_Handle
{
  Frame_Graph_Handle index;
};

enum Frame_Graph_Resource_Type
{
  FRAME_GRAPH_RESOURCE_TYPE_INVALID,
  FRAME_GRAPH_RESOURCE_TYPE_BUFFER,
  FRAME_GRAPH_RESOURCE_TYPE_TEXTURE,
  FRAME_GRAPH_RESOURCE_TYPE_ATTACHMENT,
  FRAME_GRAPH_RESOURCE_TYPE_REFERENCE,
};

struct Frame_Graph_Resource_Info
{
  core::shared_ptr<vk::Buffer> buffer;
  core::shared_ptr<vk::Image>  image;
};

struct Frame_Graph_Render_Pass
{

};

struct Frame_Graph_Resource
{
  Frame_Graph_Resource_Type type;
  struct Frame_Graph_Resource_Info resourceInfo;
  struct Frame_Graph_Node_Handle producer;
  struct Frame_Graph_Resource_Handle outputHandle;
  core::int32 refCount;
  const char* name = nullptr;
};

struct Frame_Graph_Node
{
  core::shared_ptr<vk::Render_Pass> renderPass;
  core::shared_ptr<vk::Framebuffer> framebuffer;
  Frame_Graph_Render_Pass graphRenderPass;
  core::vector<Frame_Graph_Resource_Handle> inputs;
  core::vector<Frame_Graph_Resource_Handle> outputs;
  core::vector<Frame_Graph_Node_Handle> edges;
  const char* name;
};

struct Frame_Graph;

class Frame_Graph_Builder
{
public:
  friend class Frame_Graph;
public:
  explicit Frame_Graph_Builder(core::shared_ptr<vk::Device> device);
private:
  core::shared_ptr<vk::Device> m_device;
private:
  static constexpr core::uint64 cMaxNodesCount = 1024u;
};

struct Frame_Graph
{
public:
  explicit Frame_Graph(core::shared_ptr<Renderer_Frame> rendererFrame, core::shared_ptr<Frame_Graph_Builder> builder);
  // TODO: move to utils?
  void parse(core::span<const core::byte> data);
private:
  core::shared_ptr<Renderer_Frame>            m_rendererFrame;
  core::shared_ptr<Frame_Graph_Builder>       m_builder;
  const core::vector<Frame_Graph_Node_Handle> m_nodes;
  core::string                                m_name;
};

}