module;

export module crude.scene.world;

export import crude.ecs.entity;
export import crude.scene.camera;
export import crude.scene.mesh;
export import crude.core.std_containers_heap;

export namespace crude::scene
{

class World
{
public:
  World() = default;
  void setCamera(core::shared_ptr<scene::Camera> camera) { m_camera = camera; }
  core::shared_ptr<const scene::Camera> getCamera() const { return m_camera; }
  core::shared_ptr<scene::Camera> getCamera() { return m_camera; }
  void setMesh(core::shared_ptr<scene::Mesh> mesh) { m_mesh = mesh; }
  core::shared_ptr<const scene::Mesh> getMesh() const { return m_mesh; }
  core::shared_ptr<scene::Mesh> getMesh() { return m_mesh; }
private:
  core::shared_ptr<scene::Camera>  m_camera;
  core::shared_ptr<scene::Mesh>    m_mesh;
  math::Matrix                     m_meshToModel;
};

}