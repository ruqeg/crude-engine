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
  bool                         external;
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

struct Frame_Graph_Resource_Input_Creation
{
  Frame_Graph_Resource_Type type;
  Frame_Graph_Resource_Info resource_info;
  core::string name;
};

struct Frame_Graph_Resource_Output_Creation
{
  Frame_Graph_Resource_Type type;
  Frame_Graph_Resource_Info resource_info;
  core::string              name;
};

struct Frame_Graph_Node_Creation
{
  core::vector<Frame_Graph_Resource_Input_Creation>  inputs;
  core::vector<Frame_Graph_Resource_Output_Creation> outputs;
  bool                                               enabled;
  core::string                                       name;
};

struct Frame_Graph_Node
{
  core::shared_ptr<vk::Render_Pass>          renderPass;
  core::shared_ptr<vk::Framebuffer>          framebuffer;
  Frame_Graph_Render_Pass                    graphRenderPass;
  core::vector<Frame_Graph_Resource_Handle>  inputs;
  core::vector<Frame_Graph_Resource_Handle>  outputs;
  core::vector<Frame_Graph_Node_Handle>      edges;
  core::string                               name;
  bool enabled;
};

class Frame_Graph
{
public:
  explicit Frame_Graph(core::shared_ptr<Renderer_Frame> rendererFrame);
  // TODO: move to utils?
  void parse(core::span<const core::byte> data);
  void compile();
private:
  core::shared_ptr<Frame_Graph_Node> createNode(const Frame_Graph_Node_Creation& creation);
  Frame_Graph_Resource_Handle createNodeOutput();
private:
  core::shared_ptr<Renderer_Frame>                  m_rendererFrame;
  core::vector<core::shared_ptr<Frame_Graph_Node>>  m_nodes;
  core::unordered_map<core::string, core::uint32>   m_nodeNameToIndex;
  core::string                                      m_name;
};

}