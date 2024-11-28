module crude.gfx.deferred_render_pass_scene_renderer;

import crude.gfx.vk.device;

namespace crude::gfx
{

Deferred_Render_Pass_Scene_Renderer::Deferred_Render_Pass_Scene_Renderer(core::shared_ptr<vk::Device> device)
  : m_device{ device }
{}

}