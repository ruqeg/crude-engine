#include <vulkan/vulkan.h>
#include <nlohmann/json.hpp>

module crude.gfx.frame_graph;

import crude.gfx.vk.render_pass;
import crude.gfx.vk.framebuffer;
import crude.gfx.vk.buffer;
import crude.gfx.vk.image;

namespace crude::gfx
{

Frame_Graph::Frame_Graph(core::shared_ptr<Renderer_Frame> rendererFrame)
  : m_rendererFrame{ rendererFrame }
  , m_nodes{ cMaxNodesCount, Frame_Graph_Node_Handle{} }
{}

void Frame_Graph::parse(core::span<const core::byte> data)
{
  auto stringToResourceType = [](const core::string& inputType) -> Frame_Graph_Resource_Type {
    if (inputType == "attachment") return FRAME_GRAPH_RESOURCE_TYPE_ATTACHMENT;
    if (inputType == "buffer") return FRAME_GRAPH_RESOURCE_TYPE_BUFFER;
    if (inputType == "reference") return FRAME_GRAPH_RESOURCE_TYPE_REFERENCE;
    if (inputType == "texture") return FRAME_GRAPH_RESOURCE_TYPE_TEXTURE;
    assert(false);
    return FRAME_GRAPH_RESOURCE_TYPE_INVALID;
  };

  const nlohmann::json graph = nlohmann::json::parse(data);

  m_name = graph.value("name", core::string{});
  
  const nlohmann::json passes = graph["passes"];
  for (const nlohmann::json pass: passes)
  {
    const nlohmann::json passInputs = pass["inputs"];
    const nlohmann::json passOutputs = pass["outputs"];

    Frame_Graph_Node_Creation nodeCreation{ };
    nodeCreation.inputs.reserve(passInputs.size());
    nodeCreation.outputs.reserve(passOutputs.size());

    for (const nlohmann::json passInput: passInputs)
    {
      Frame_Graph_Resource_Input_Creation inputCreation{};
      inputCreation.type = stringToResourceType(passInput.value("type", core::string{}));
      inputCreation.name = passInput.value("name", core::string{});
      inputCreation.resource_info.external = false;
      
      assert(!inputCreation.name.empty());

      nodeCreation.inputs.push_back(inputCreation);
    }

    for (const nlohmann::json passOutput : passOutputs)
    {
      Frame_Graph_Resource_Output_Creation outputCreation{ };
      outputCreation.type = stringToResourceType(passOutput.value("type", core::string{}));
      outputCreation.name = passOutput.value("name", core::string{});

      switch (outputCreation.type)
      {
      case FRAME_GRAPH_RESOURCE_TYPE_ATTACHMENT:
      case FRAME_GRAPH_RESOURCE_TYPE_TEXTURE:
      {
        //outputCreation.resource_info.texture.format = /* TODO with hashmap*/;
        //outputCreation.resource_info.texture.load_op =/* TODO with hashmap*/;

        const nlohmann::json resolution = passOutput["resolution"];

        //outputCreation.resource_info.texture.width = resolution[0];
        //outputCreation.resource_info.texture.height = resolution[1];
        //outputCreation.resource_info.texture.depth = 1;
        break;
      }
      case FRAME_GRAPH_RESOURCE_TYPE_BUFFER:
      {
        assert(false);
        break;
      }
      }

      assert(!outputCreation.name.empty());

      nodeCreation.outputs.push_back(outputCreation);
    }

    nodeCreation.name = pass.value("name", core::string{});
    nodeCreation.enabled = pass.value("enabled", true);
    Frame_Graph_Node_Handle node_handle = createNode(nodeCreation);
    m_nodes.push_back(node_handle);
  }
}

core::shared_ptr<Frame_Graph_Node> Frame_Graph::createNode(const Frame_Graph_Node_Creation& creation)
{
  core::shared_ptr<Frame_Graph_Node> node = core::allocateShared<Frame_Graph_Node>();
  node->name    = creation.name;
  node->enabled = creation.enabled;
  node->inputs.reserve(creation.inputs.size());
  node->outputs.reserve(creation.outputs.size());
  node->edges.reserve(creation.outputs.size());
  node->framebuffer;
  node->renderPass;

  m_nodeNameToIndex[node->name] = 0;

  for (const Frame_Graph_Resource_Output_Creation& outputCreation: creation.outputs)
  {
    Frame_Graph_Resource_Handle output = create_node_output(output_creation, node_handle);
    node->outputs.push_back(output);
  }

  for (const Frame_Graph_Resource_Input_Creation& inputsCreation : creation.inputs)
  {
    FrameGraphResourceHandle input_handle = create_node_input(inputsCreation);
    node->inputs.push_back(input_handle);
  }

  return node;
}

Frame_Graph_Resource_Handle Frame_Graph::createNodeOutput()
{
  FrameGraphResourceHandle resource_handle{ k_invalid_index };
  resource_handle.index = resource_cache.resources.obtain_resource();

  if (resource_handle.index == k_invalid_index) {
    return resource_handle;
  }

  FrameGraphResource* resource = resource_cache.resources.get(resource_handle.index);
  resource->name = creation.name;
  resource->type = creation.type;

  if (creation.type != FrameGraphResourceType_Reference) {
    resource->resource_info = creation.resource_info;
    resource->output_handle = resource_handle;
    resource->producer = producer;
    resource->ref_count = 0;

    resource_cache.resource_map.insert(hash_bytes((void*)resource->name, strlen(creation.name)), resource_handle.index);
  }

  return resource_handle;
}

}