module;

#include <vulkan/vulkan.hpp>
#include <directxmath/DirectXMath.h>
#include <flecs.h>

export module crude.graphics.renderer;

export import crude.core.alias;
export import crude.graphics.program;

export namespace crude::graphics
{

class Renderer
{
public:
  Renderer(core::shared_ptr<Program> program)
    : m_program(program) {}
  ~Renderer() = default;
public:
  void drawFrame() { m_program->drawFrame(); }
  flecs::entity_view getNode() { m_program->getNode(); }
private:
  core::shared_ptr<Program> m_program;
};

}