module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.model_buffer;

export import crude.core.shared_ptr;
export import crude.scene.model_geometry;
export import crude.graphics.buffer;
export import crude.graphics.device_memory;
export import crude.graphics.command_pool;
export import crude.graphics.queue;

import crude.core.std_containers_stack;

export namespace crude::graphics
{

class Model_Buffer
{
private:
  enum Buffer_Additional_Usage
  {
    BUFFER_ADDITIONAL_USAGE_INDEX_BUFFER = VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
    BUFFER_ADDITIONAL_USAGE_VERTEX_BUFFER = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
  };
public:
  Model_Buffer(core::Shared_Ptr<Queue> queue, core::Shared_Ptr<Command_Pool> commandPool, const scene::Model_Geometry& modelGeometry);
private:
  void initalizeBuffer(core::Shared_Ptr<Buffer>         outBuffer, 
                       core::Shared_Ptr<Device_Memory>  outBufferMemory, 
                       const core::span<const void>&    inData, 
                       Buffer_Additional_Usage          inAdditionalUsage);
private:
  core::Shared_Ptr<Queue>          m_queue;
  core::Shared_Ptr<Command_Pool>   m_commandPool;
  core::Shared_Ptr<Buffer>         m_vertexBuffer;
  core::Shared_Ptr<Device_Memory>  m_vertexBufferMemory;
  core::Shared_Ptr<Buffer>         m_indexBuffer;
  core::Shared_Ptr<Buffer>         m_indexBufferMemory;
};

}
