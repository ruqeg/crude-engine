module;

export module crude.scene.world;

export import crude.ecs.entity;
export import crude.scene.camera;
export import crude.scene.image;
export import crude.scene.mesh;
export import crude.graphics.sampler_state;
export import crude.core.std_containers_heap;

export namespace crude::scene
{
// !TODO

class Sampler
{

};

class Texture
{
public:
  Texture(core::shared_ptr<Image> image, core::shared_ptr<graphics::Sampler_State> samplerState)
    : m_image(image), m_samplerState(samplerState) {}
  ~Texture() = default;
public:
  core::shared_ptr<const Image> getImage() const { return m_image; }
  core::shared_ptr<const graphics::Sampler_State> getSamplerState() const { return m_samplerState; }
private:
  core::shared_ptr<Image>                    m_image;
  core::shared_ptr<graphics::Sampler_State>  m_samplerState;
};

class World
{
public:
  World() = default;
  void setCamera(core::shared_ptr<scene::Camera> camera) { m_camera = camera; }
  core::shared_ptr<const scene::Camera> getCamera() const { return m_camera; }
  void setTexture(core::shared_ptr<Texture> texture) { m_texture = texture;  }
  core::shared_ptr<const Texture> getTexture() const { return m_texture; }
  core::shared_ptr<scene::Camera> getCamera() { return m_camera; }
  void setMesh(core::shared_ptr<scene::Mesh> mesh) { m_mesh = mesh; }
  core::shared_ptr<const scene::Mesh> getMesh() const { return m_mesh; }
  core::shared_ptr<scene::Mesh> getMesh() { return m_mesh; }
private:
  core::shared_ptr<scene::Camera>  m_camera;
  core::shared_ptr<scene::Mesh>    m_mesh;
  math::Matrix                     m_meshToModel;
  core::shared_ptr<Texture>        m_texture;
};

}