module;

#include <DirectXMath.h>
#include <flecs.h>
#include <nlohmann/json.hpp>

export module crude.editor.resources.scene_to_json;

export import crude.core.std_containers_heap;



import crude.scene.transform;
import crude.scene.light;

export namespace crude::editor::resources
{

void registerTransformSerializationSupport(flecs::world& world) {
  world.component<scene::Transform>()
    // Serialize as struct
    .opaque(world.component()
      .member<DirectX::XMFLOAT3>("translation")
      .member<DirectX::XMFLOAT4>("rotation")
      .member<DirectX::XMFLOAT3>("scale"))
    // Forward struct members to serializer
    .serialize([](const flecs::serializer* s, const scene::Transform* transform) {
      auto& translation = transform->getTranslationFloat3();
      auto& rotation = transform->getRotationFloat4();
      auto& scale = transform->getScaleFloat3();
      s->member("translation");
      s->value(translation.x);
      s->value(translation.y);
      s->value(translation.z);
      s->member("rotation");
      s->value(rotation.x);
      s->value(rotation.y);
      s->value(rotation.z);
      s->value(rotation.w);
      s->member("scale");
      s->value(scale.x);
      s->value(scale.y);
      s->value(scale.z);
      return 0;
      });
}

void registerPointLightCPUSerializationSupport(flecs::world& world) {
  world.component<scene::Point_Light_CPU>()
    // Serialize as struct
    .opaque(world.component()
      .member<DirectX::XMFLOAT3>("intensity")
      .member<core::float32>("sourceRadius"))
    // Forward struct members to serializer
    .serialize([](const flecs::serializer* s, const scene::Point_Light_CPU* pointLightCPU) {
      s->member("intensity");
      s->value(pointLightCPU->intensity.x);
      s->value(pointLightCPU->intensity.y);
      s->value(pointLightCPU->intensity.z);
      s->member("sourceRadius");
      s->value(pointLightCPU->sourceRadius);
      return 0;
      });
}

void registerComponentsSerialization(flecs::world world)
{
  registerTransformSerializationSupport(world);
  registerPointLightCPUSerializationSupport(world);
}

nlohmann::json sceneToJson(flecs::entity scene)
{
  nlohmann::json sceneChildrenJson;

  nlohmann::json* pParentNodeJson = &sceneChildrenJson;
  auto parseNodeToJson = [&pParentNodeJson](this const auto& parseNodeToJson, flecs::entity node) {
    nlohmann::json* pOldParentNodeJson = pParentNodeJson;

    nlohmann::json nodeChildrenJson;
    pParentNodeJson = &nodeChildrenJson;
    node.children(parseNodeToJson);
    pParentNodeJson = pOldParentNodeJson;

    (*pParentNodeJson)["node"]     = node.to_json();
    (*pParentNodeJson)["children"] = nodeChildrenJson;
    };

  parseNodeToJson(scene);
  
  nlohmann::json sceneJson;
  sceneJson["node"]     = scene.to_json();
  sceneJson["children"] = sceneChildrenJson;
  return sceneJson;
}

}