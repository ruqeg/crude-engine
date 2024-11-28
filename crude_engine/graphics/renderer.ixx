module;

export module crude.gfx.deferred_render_pass_scene_renderer;

export import crude.core.std_containers_heap;

export namespace crude::gfx::vk
{

class Device;

}

export namespace crude::gfx
{

class Deferred_Render_Pass_Scene_Renderer
{
public:
  explicit Deferred_Render_Pass_Scene_Renderer(core::shared_ptr<vk::Device> device);
  void render();
private:
  core::shared_ptr<vk::Device> m_device;
};

}