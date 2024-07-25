module;

#include <flecs.h>

export module crude.scene.scene;

export import crude.scene.camera;
export import crude.scene.image;
export import crude.scene.node;
export import crude.scene.mesh;
export import crude.core.std_containers_heap;

export namespace crude::scene
{

class Scene
{
public:
  void addNode(core::shared_ptr<Node> node) { m_nodes.push_back(node); }
  void setNodes(const core::vector<core::shared_ptr<Node>>& nodes) { m_nodes = nodes; }
  const core::vector<core::shared_ptr<Node>>& getNodes() const { return m_nodes; }
  flecs::world& getWorld() { return m_world; }
private:
  core::vector<core::shared_ptr<Node>>  m_nodes;
  core::shared_ptr<Node>                m_root;
  flecs::world                          m_world;
};

}