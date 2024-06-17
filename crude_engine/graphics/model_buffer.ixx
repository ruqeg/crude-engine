module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.model_buffer;

export import crude.scene.model_geometry;

export namespace crude::graphics
{

class Command_Buffer;
class Index_Buffer;
class Vertex_Buffer;

class Model_Buffer
{
public:
  Model_Buffer(core::shared_ptr<Command_Buffer>  commandBuffer,
               const scene::Model_Geometry&      modelGeometry);
  core::shared_ptr<Vertex_Buffer> getVertexBuffer();
  core::shared_ptr<Index_Buffer> getIndexBuffer();
private:
  core::shared_ptr<Vertex_Buffer>  m_vertexBuffer;
  core::shared_ptr<Index_Buffer>   m_indexBuffer;
};

}
