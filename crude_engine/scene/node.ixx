module;

#include <typeindex>

export module crude.scene.node;

export import crude.scene.transform;
export import crude.scene.component;
export import crude.core.std_containers_heap;

export namespace crude::scene
{

class Node
{
public:
  void setTransform(const Transform& transform) { m_transform = transform; }
  const Transform& getTransform() const { return m_transform; }
  Transform& getTransform() { return m_transform; }
  core::shared_ptr<Node> getParent() { return m_parent; }
  core::shared_ptr<const Node> getParent() const { return m_parent; }
private:
  core::u8string                                                  m_name;
  core::shared_ptr<Node>                                          m_parent;
  core::vector<core::shared_ptr<Node>>                            m_children;
  core::unordered_map<std::type_index, core::shared_ptr<Node[]>>  m_typeToComponents;
  Transform                                                       m_transform;
};

}