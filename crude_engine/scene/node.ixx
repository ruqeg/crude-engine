module;

#include <typeindex>
#include <flecs.h>

export module crude.scene.node;

export import crude.scene.transform;
export import crude.core.std_containers_heap;

export namespace crude::scene
{

class Scene;

class Node
{
public:
  explicit Node(core::shared_ptr<Scene> scene, const char* name);
public:
  flecs::entity& getEntity() { return m_entity; }
public:
  void addChild(core::shared_ptr<Node> child) { m_children.push_back(child); }
  const core::vector<core::shared_ptr<Node>>& getChildren() const { return m_children; }
  void setParent(core::shared_ptr<Node> parent) { m_parent = parent; }
  core::shared_ptr<Node> getParent() { return m_parent; }
  core::shared_ptr<const Node> getParent() const { return m_parent; }
private:
  core::shared_ptr<Node>                m_parent;
  core::vector<core::shared_ptr<Node>>  m_children;
  flecs::entity                         m_entity;
};

}