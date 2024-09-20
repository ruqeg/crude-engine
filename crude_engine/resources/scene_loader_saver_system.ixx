module;

#include <DirectXMath.h>
#include <flecs.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <functional>

export module crude.resources.scene_loader_saver_system;

export import crude.core.std_containers_heap;

export namespace crude::resources
{

using Scene_Saver_Callback_Function  = std::function<void(flecs::entity)>;
using Scene_Loader_Callback_Function = std::function<void(flecs::entity oldEntity, flecs::entity newEntity)>;

struct Scene_Saver_Component
{
  core::string path;
};

struct Scene_Loader_Component
{
  flecs::entity sceneNode;
  core::string  path;
};

struct Scene_Loader_Context
{
  Scene_Loader_Callback_Function callback;
};

struct Scene_Saver_Context
{
  Scene_Saver_Callback_Function callback;
};

[[nodiscard]] flecs::system registerSceneLoaderSystem(flecs::world world, core::shared_ptr<Scene_Loader_Context> ctx);
[[nodiscard]] flecs::system registerSceneSaverSystem(flecs::world world, core::shared_ptr<Scene_Saver_Context> ctx);

}

namespace crude::resources
{

// 0 - Scene_Loader_Component
void sceneLoaderSystemProcess(flecs::iter& it);

// 0 - Scene_Saver_Component
void sceneSaverSystemProcess(flecs::iter& it);

void saveScene(flecs::entity sceneNode, const core::string& path);
flecs::entity loadScene(flecs::world world, const core::string& path);

void parseNodeToJsonWithoutChildren(flecs::entity node, nlohmann::json& nodeJson);
void parseNodeFromJsonWithoutChildren(flecs::entity node, const nlohmann::json& nodeJson);
flecs::entity createNodeFromJsonWithChildren(flecs::world world, const nlohmann::json& nodeJson);

}