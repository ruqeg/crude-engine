module;

#include <flecs.h>

export module application;

export import crude.engine;
export import gui.imgui_editor_layout_draw_system;

export
{

class Application : public crude::Engine
{
public:
  void initialize();
  void run();
  void deinitialize();
private:
  void initializeScene(crude::core::float32 aspectRatio);
  void initializeEditorCamera(crude::core::float32 aspectRatio);
private:
  flecs::system                                               m_freeCameraUpdateSystem;
  crude::core::shared_ptr<gui::Imgui_Editor_Layout_Draw_Ctx>  m_editorLayoutCtx;
};

}