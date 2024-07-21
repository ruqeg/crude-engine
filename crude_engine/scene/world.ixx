module;

export module crude.scene.world;

export import crude.ecs.entity;
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
  void setTexture(core::shared_ptr<Texture> texture) { m_texture = texture;  }
  core::shared_ptr<const Texture> getTexture() const { return m_texture; }
  core::shared_ptr<scene::Camera> getCamera() { return m_camera; }
  void setGeometry(core::shared_ptr<scene::Geometry> geometry) { m_geometry = geometry; }
  core::shared_ptr<const scene::Geometry> getGeometry() const { return m_geometry; }
  core::shared_ptr<scene::Geometry> getGeometry() { return m_geometry; }
private:
  core::shared_ptr<scene::Camera>    m_camera;
  core::shared_ptr<scene::Geometry>  m_geometry;
  math::Matrix                       m_meshToModel;
  core::shared_ptr<Texture>          m_texture;
};

}