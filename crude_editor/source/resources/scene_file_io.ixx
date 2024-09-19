module;

#include <DirectXMath.h>
#include <flecs.h>
#include <nlohmann/json.hpp>
#include <fstream>

export module crude.editor.resources.scene_file_io;

export import crude.core.std_containers_heap;

export namespace crude::graphics
{

class Command_Pool;

}

export namespace crude::editor::resources
{

class Scene_File_IO
{
public:
  explicit Scene_File_IO(core::shared_ptr<graphics::Command_Pool> commandPool);
public:
  void save(flecs::entity sceneNode, const char* path);
  flecs::entity load(flecs::world world, const char* path);
private:
  void parseNodeToJsonWithoutChildren(flecs::entity node, nlohmann::json& nodeJson);
  void parseNodeFromJsonWithoutChildren(flecs::entity node, const nlohmann::json& nodeJson);
  flecs::entity createNodeFromJsonWithChildren(flecs::world world, const nlohmann::json& nodeJson);
private:
  core::shared_ptr<graphics::Command_Pool> m_commandPool;
};


}