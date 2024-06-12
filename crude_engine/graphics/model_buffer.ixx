module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.model_buffer;

export import crude.scene.model_geometry;
export import crude.graphics.index_buffer;
export import crude.graphics.vertex_buffer;

export namespace crude::graphics
{

class Model_Buffer
{
public:
  Model_Buffer(core::shared_ptr<Command_Pool>  commandPool, 
               const scene::Model_Geometry&    modelGeometry);
private:
  core::shared_ptr<Vertex_Buffer>  m_vertexBuffer;
  core::shared_ptr<Index_Buffer>   m_indexBuffer;

  friend class Command_Buffer;
};

}
