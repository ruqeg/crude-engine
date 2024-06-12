module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.model_buffer;

export import crude.scene.model_geometry;
export import crude.graphics.buffer_dependent;

export namespace crude::graphics
{

class Model_Buffer
{
public:
  Model_Buffer(core::shared_ptr<Queue>         queue, 
               core::shared_ptr<Command_Pool>  commandPool, 
               const scene::Model_Geometry&    modelGeometry);
private:
  core::shared_ptr<Buffer_Dependent<scene::Vertex_GPU>>           m_vertexBuffer;
  core::shared_ptr<Buffer_Dependent<scene::Index_Triangle_GPU>>   m_indexBuffer;

  friend class Command_Buffer;
};

}
