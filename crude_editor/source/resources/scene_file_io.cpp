#include <DirectXMath.h>
#include <flecs.h>
#include <nlohmann/json.hpp>
#include <fstream>

module crude.editor.resources.scene_file_io;

import crude.graphics.command_pool;
import crude.scene.transform;
import crude.scene.light;
import crude.resources.gltf_loader;

namespace DirectX
{

void to_json(nlohmann::json& j, const DirectX::XMFLOAT3& float3)
{
  crude::core::array<crude::core::float32, 3> arr = { float3.x, float3.y, float3.z };
  j = arr;
}

void to_json(nlohmann::json& j, const DirectX::XMFLOAT4& float4)
{
  crude::core::array<crude::core::float32, 4> arr = { float4.x, float4.y, float4.z, float4.w };
  j = arr;
}

void from_json(const nlohmann::json& j, DirectX::XMFLOAT3& float3)
{
  float3.x = j[0];
  float3.y = j[1];
  float3.z = j[2];
}

void from_json(const nlohmann::json& j, DirectX::XMFLOAT4& float4)
{
  float4.x = j[0];
  float4.y = j[1];
  float4.z = j[2];
  float4.w = j[3];
}

}

namespace crude::scene
{

void to_json(nlohmann::json& j, const Transform& transform)
{
  j = nlohmann::json
  {
    { "translation", transform.getTranslationFloat3() },
    { "rotation", transform.getRotationFloat4() },
    { "scale", transform.getScaleFloat3() },
  };
}

void from_json(const nlohmann::json& j, Transform& transform)
{
  const DirectX::XMFLOAT3 translation = j["translation"].get<DirectX::XMFLOAT3>();
  const DirectX::XMFLOAT4 rotation = j["rotation"].get<DirectX::XMFLOAT4>();
  const DirectX::XMFLOAT3 scale = j["scale"].get<DirectX::XMFLOAT3>();
  transform.setTranslation(translation);
  transform.setRotationQuaternion(DirectX::XMLoadFloat4(&rotation));
  transform.setScale(scale);
}

void to_json(nlohmann::json& j, const Point_Light_CPU& pointLightCPU)
{
  j = nlohmann::json
  {
    { "intensity", pointLightCPU.intensity },
    { "sourceRadius", pointLightCPU.sourceRadius },
  };
}

void from_json(const nlohmann::json& j, Point_Light_CPU& pointLightCPU)
{
  pointLightCPU.intensity = j["intensity"].get<DirectX::XMFLOAT3>();
  pointLightCPU.sourceRadius = j["sourceRadius"];
}

}

namespace crude::resources
{

void to_json(nlohmann::json& j, const GLTF_Model& gltfModel)
{
  j = nlohmann::json
  {
    { "path", gltfModel.path },
  };
}

void from_json(const nlohmann::json& j, GLTF_Model& gltfModel)
{
  gltfModel.path = j["path"];
}

}

namespace crude::editor::resources
{

Scene_File_IO::Scene_File_IO(core::shared_ptr<graphics::Command_Pool> commandPool)
  : m_commandPool{ commandPool }
{}

void Scene_File_IO::save(flecs::entity sceneNode, const char* path)
{
  nlohmann::json sceneJson;
  nlohmann::json* entityParentJson;
  auto parseNodeChildrenToJson = [this, &entityParentJson](this const auto& parseNodeChildrenToJson, flecs::entity node) {
    nlohmann::json entityJson;
    parseNodeToJsonWithoutChildren(node, entityJson);

    if (node.has<crude::resources::GLTF_Model>())
    {
      return;
    }

    nlohmann::json* oldEntityParentJson = entityParentJson;
    entityParentJson = &entityJson;
    node.children(parseNodeChildrenToJson);
    entityParentJson = oldEntityParentJson;

    (*entityParentJson)["children"].push_back(entityJson);
    };

  entityParentJson = &sceneJson;
  parseNodeToJsonWithoutChildren(sceneNode, sceneJson);
  if (!sceneNode.has<crude::resources::GLTF_Model>())
  {
    sceneNode.children(parseNodeChildrenToJson);
  }

  std::ofstream otf(path);
  otf << std::setw(4) << sceneJson << std::endl;
}

flecs::entity Scene_File_IO::load(flecs::world world, const char* path)
{
  std::ifstream inf(path);
  nlohmann::json sceneJson = nlohmann::json::parse(inf);
  inf.close();
  return createNodeFromJsonWithChildren(world, sceneJson);
}

flecs::entity Scene_File_IO::createNodeFromJsonWithChildren(flecs::world world, const nlohmann::json& nodeJson)
{
  flecs::entity node = world.entity();
  parseNodeFromJsonWithoutChildren(node, nodeJson);

  if (!nodeJson.contains("children"))
  {
    return node;
  }

  const nlohmann::json& childrenJson = nodeJson["children"];
  for (const nlohmann::json& childJson : childrenJson)
  {
    flecs::entity child = createNodeFromJsonWithChildren(world, childJson);
    child.child_of(node);
  }

  return node;
}

void Scene_File_IO::parseNodeFromJsonWithoutChildren(flecs::entity node, const nlohmann::json& nodeJson)
{
  node.set_name(nodeJson["name"].get<std::string>().c_str());

  const nlohmann::json& nodeComponentsJson = nodeJson["components"];
  if (nodeComponentsJson.contains("transform"))
  {
    scene::Transform transform = nodeComponentsJson["transform"].get<scene::Transform>();
    transform.setNode(node);
    node.set<scene::Transform>(transform);
  }
  if (nodeComponentsJson.contains("point_light"))
  {
    node.set<scene::Point_Light_CPU>(nodeComponentsJson["point_light"].get<scene::Point_Light_CPU>());
  }
  if (nodeComponentsJson.contains("gltf_model"))
  {
    crude::resources::GLTF_Model gltfModel = nodeComponentsJson["gltf_model"].get<crude::resources::GLTF_Model>();
    node.set<crude::resources::GLTF_Model>(gltfModel);

    //crude::resources::GLTF_Loader gltfLoader(node.world(), m_commandPool);
    //gltfLoader.loadToNodeFromFile(node, gltfModel.path.c_str());
  }
}

void Scene_File_IO::parseNodeToJsonWithoutChildren(flecs::entity node, nlohmann::json& nodeJson)
{
  nodeJson["name"] = node.name();

  nlohmann::json nodeComponentsJson;
  if (node.has<scene::Transform>())
  {
    nodeComponentsJson["transform"] = *node.get<scene::Transform>();
  }
  if (node.has<scene::Point_Light_CPU>())
  {
    nodeComponentsJson["point_light"] = *node.get<scene::Point_Light_CPU>();
  }
  if (node.has<crude::resources::GLTF_Model>())
  {
    nodeComponentsJson["gltf_model"] = *node.get<crude::resources::GLTF_Model>();
  }
  nodeJson["components"] = nodeComponentsJson;
}

}