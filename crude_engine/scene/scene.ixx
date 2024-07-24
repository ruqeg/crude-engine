module;

export module crude.scene.world;

export import crude.scene.camera;
export import crude.scene.image;
export import crude.scene.mesh;
export import crude.core.std_containers_heap;

export namespace crude::scene
{

struct Visual
{
  core::unordered_map<core::uint32, core::uint32> meshIndexToMaterialIndex;
};

// !TODO
class World
{
public:
  World() = default;
  void setCamera(core::shared_ptr<scene::Camera> camera) { m_camera = camera; }
  core::shared_ptr<const scene::Camera> getCamera() const { return m_camera; }
  core::shared_ptr<scene::Camera> getCamera() { return m_camera; }
  void setNodes(const core::vector<core::shared_ptr<scene::Node>>& nodes) { m_nodes = nodes; }
  const core::vector<core::shared_ptr<scene::Node>>& getNodes() const { return m_nodes; }
  core::vector<core::shared_ptr<scene::Node>>& getNodes() { return m_nodes; }
  void setGeometry(core::shared_ptr<scene::Geometry> geometry) { m_geometry = geometry; }
  core::shared_ptr<const scene::Geometry> getGeometry() const { return m_geometry; }
  core::shared_ptr<scene::Geometry> getGeometry() { return m_geometry; }
  void setTextures(const core::vector<core::shared_ptr<scene::Texture>>& textures) { m_textures = textures; }
  const core::vector<core::shared_ptr<scene::Texture>>& getTextures() const { return m_textures; }
  core::vector<core::shared_ptr<scene::Texture>>& getTextures() { return m_textures; }
private:
  core::shared_ptr<scene::Camera>    m_camera;
  core::vector<core::shared_ptr<scene::Node>>      m_nodes;
  core::shared_ptr<scene::Geometry>  m_geometry;
  core::vector<core::shared_ptr<scene::Texture>>   m_textures;
public:
  core::shared_ptr<Visual> m_visual;
};

}