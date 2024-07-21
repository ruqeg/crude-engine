module;

export module crude.scene.world;

export import crude.scene.camera;
export import crude.scene.image;
export import crude.scene.mesh;
export import crude.core.std_containers_heap;

export namespace crude::scene
{
// !TODO
class World
{
public:
  World() = default;
  void setCamera(core::shared_ptr<scene::Camera> camera) { m_camera = camera; }
  core::shared_ptr<const scene::Camera> getCamera() const { return m_camera; }
  core::shared_ptr<scene::Camera> getCamera() { return m_camera; }
  void setNode(core::shared_ptr<scene::Node> node) { m_node = node; }
  core::shared_ptr<const scene::Node> getNode() const { return m_node; }
  core::shared_ptr<scene::Node> getNode() { return m_node; }
private:
  core::shared_ptr<scene::Camera>    m_camera;
  core::shared_ptr<scene::Node>      m_node;
};

}