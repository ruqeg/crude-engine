module;

#include <flecs.h>

export module crude.editor.application;

export import crude.engine;
export import crude.editor.gui.imgui_editor_layout_draw_system;

export namespace crude::editor
{

class Application : public Engine
{
public:
  void initialize();
  void run();
  void deinitialize();
private:
  void initializeScene(core::float32 aspectRatio);
  void initializeEditorCamera(core::float32 aspectRatio);
private:
  flecs::system                                        m_freeCameraUpdateSystem;
  core::shared_ptr<gui::Imgui_Editor_Layout_Draw_Ctx>  m_editorLayoutCtx;
  flecs::entity                                        m_cameraNode;
};

}