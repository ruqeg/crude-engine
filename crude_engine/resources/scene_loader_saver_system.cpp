#include <DirectXMath.h>
#include <flecs.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <functional>

module crude.resources.scene_loader_saver_system;

import crude.gfx.vk.command_pool;
import crude.scene.transform;
import crude.scene.light;
import crude.resources.gltf_loader_system;


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

void to_json(nlohmann::json& j, const GLTF_Metadata_Component& gltfModel)
{
  j = nlohmann::json
  {
    { "path", gltfModel.path.string() },
  };
}

void from_json(const nlohmann::json& j, GLTF_Metadata_Component& gltfModel)
{
  gltfModel.path = j["path"].get<std::string>();
}

}

namespace crude::resources
{

flecs::system registerSceneLoaderSystem(flecs::world world, core::shared_ptr<Scene_Loader_Context> ctx)
{
  return world.system<Scene_Loader_Component>("SceneLoaderSystem")
    .ctx(ctx.get())
    .kind(flecs::OnLoad)
    .run(sceneLoaderSystemProcess);
}

flecs::system registerSceneSaverSystem(flecs::world world, core::shared_ptr<Scene_Saver_Context> ctx)
{
  return world.system<Scene_Saver_Component>("SceneSaverSystem")
    .ctx(ctx.get())
    .kind(flecs::OnLoad)
    .run(sceneSaverSystemProcess);
}

// 0 - Scene_Loader_Component
void sceneLoaderSystemProcess(flecs::iter& it)
{
  Scene_Loader_Context* ctx = it.ctx<Scene_Loader_Context>();
  while (it.next())
  {
    auto sceneLoader = it.field<Scene_Loader_Component>(0);

    for (auto i : it)
    {
      core::string path = sceneLoader[i].path;
      it.entity(i).destruct();
      ctx->callback(loadScene(it.world(), path));
    }
  }
}

// 0 - Scene_Saver_Component
void sceneSaverSystemProcess(flecs::iter& it)
{
  Scene_Saver_Context* ctx = it.ctx<Scene_Saver_Context>();
  while (it.next())
  {
    auto sceneSaver = it.field<Scene_Saver_Component>(0);

    for (auto i : it)
    {
      saveScene(it.entity(i), sceneSaver[i].path);
      it.entity(i).remove<Scene_Saver_Component>();
      ctx->callback(it.entity(i));
    }
  }
}

void saveScene(flecs::entity sceneNode, const core::string& path)
{
  nlohmann::json sceneJson;
  nlohmann::json* entityParentJson;
  auto parseNodeChildrenToJson = [&entityParentJson](this const auto& parseNodeChildrenToJson, flecs::entity node) {
    nlohmann::json entityJson;
    parseNodeToJsonWithoutChildren(node, entityJson);

    if (!node.has<crude::resources::GLTF_Metadata_Component>())
    {
      nlohmann::json* oldEntityParentJson = entityParentJson;
      entityParentJson = &entityJson;
      node.children(parseNodeChildrenToJson);
      entityParentJson = oldEntityParentJson;
    }

    (*entityParentJson)["children"].push_back(entityJson);
    };

  entityParentJson = &sceneJson;
  parseNodeToJsonWithoutChildren(sceneNode, sceneJson);
  if (!sceneNode.has<crude::resources::GLTF_Metadata_Component>())
  {
    sceneNode.children(parseNodeChildrenToJson);
  }

  std::ofstream otf(path.c_str());
  otf << std::setw(4) << sceneJson << std::endl;
}

flecs::entity loadScene(flecs::world world, const core::string& path)
{
  std::ifstream inf(path.c_str());
  nlohmann::json sceneJson = nlohmann::json::parse(inf);
  inf.close();
  return createNodeFromJsonWithChildren(world, sceneJson);
}

flecs::entity createNodeFromJsonWithChildren(flecs::world world, const nlohmann::json& nodeJson)
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

void parseNodeFromJsonWithoutChildren(flecs::entity node, const nlohmann::json& nodeJson)
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
  if (nodeComponentsJson.contains("gltf"))
  {
    node.set<crude::resources::GLTF_Metadata_Component>(nodeComponentsJson["gltf"].get<crude::resources::GLTF_Metadata_Component>());
    node.add<crude::resources::GLTF_Loader_Uninitialized_Flag>();
  }
}

void parseNodeToJsonWithoutChildren(flecs::entity node, nlohmann::json& nodeJson)
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
  if (node.has<crude::resources::GLTF_Metadata_Component>())
  {
    nodeComponentsJson["gltf"] = *node.get<crude::resources::GLTF_Metadata_Component>();
  }
  nodeJson["components"] = nodeComponentsJson;
}

}