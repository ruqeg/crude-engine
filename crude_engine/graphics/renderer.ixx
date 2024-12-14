/*module;

export module crude.gfx.deferred_render_pass_scene_renderer;

export import crude.core.std_containers_heap;
export import crude.gfx.vk.descriptor_set_layout_binding;
export import crude.gfx.render_graph;

export namespace crude::gfx::vk
{

class Device;
class Pipeline;
class Render_Pass;

}

export namespace crude::gfx
{

class Deferred_Scene_Renderer
{
public:
  explicit Deferred_Scene_Renderer(core::shared_ptr<vk::Device> device, core::shared_ptr<Render_Pass> renderPass);
  void render();
private:
  core::shared_ptr<vk::Device>                     m_device;
  core::shared_ptr<vk::Pipeline>                   m_pipeline;
  core::shared_ptr<vk::Render_Pass>                m_renderPass;
};

}*/