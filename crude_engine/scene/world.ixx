module;

export module crude.scene.world;

export import crude.ecs.entity;
export import crude.scene.camera;
export import crude.scene.model_geometry;
export import crude.core.std_containers_heap;

export namespace crude::scene
{

class World
{
public:
  World(const core::vector<scene::Camera>& cameras)
    : m_cameras(cameras) {}
  Camera& getCamera() noexcept { return m_cameras.front(); }
private:
  core::vector<scene::Camera>  m_cameras;
};

}