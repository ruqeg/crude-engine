#include <vulkan/vulkan.h>
#include <nlohmann/json.hpp>

module crude.gfx.frame_graph;

import crude.gfx.vk.render_pass;
import crude.gfx.vk.framebuffer;
import crude.gfx.vk.buffer;
import crude.gfx.vk.image;

namespace crude::gfx
{

Frame_Graph_Builder::Frame_Graph_Builder(core::shared_ptr<vk::Device> device)
  : m_device{ device }
{}

Frame_Graph::Frame_Graph(core::shared_ptr<Renderer_Frame> rendererFrame, core::shared_ptr<Frame_Graph_Builder> builder)
  : m_rendererFrame{ rendererFrame }
  , m_builder{ builder }
  , m_nodes{ Frame_Graph_Builder::cMaxNodesCount, Frame_Graph_Node_Handle{} }
{}

void Frame_Graph::parse(core::span<const core::byte> data)
{
  const nlohmann::json graph = nlohmann::json::parse(data);

  m_name = graph.value("name", "");

  const nlohmann::json passes = graph["passes"];
  for (const nlohmann::json pass: passes)
  {
    const nlohmann::json passInputs = pass["inputs"];
    const nlohmann::json passOutputs = pass["outputs"];

}

}